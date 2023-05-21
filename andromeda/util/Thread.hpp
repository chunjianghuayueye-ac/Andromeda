#ifndef ANDROMEDA_UTIL_THREAD
#define ANDROMEDA_UTIL_THREAD

#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace andromeda {
	namespace util {
		enum ThreadWorkMode
		{
			Join, Detach
		};
		//处于Stopped时_thread=nullptr，只能重新调用start()执行
		enum ThreadState
		{
			Stopped, Running, Suspended
		};

		template<typename Thread>
		static inline void exitThread(Thread* thread)
		{
			thread->exit();
		}

		template<typename Callable,typename Derived=void> //Derived默认为空，继承本类时需要指定Derived为子类
		class Thread
		{
		private:
			friend void exitThread<>(Thread<Callable>* thread);

			std::thread* _thread=nullptr;
			ThreadWorkMode _workState=Detach;
			ThreadState _state=Stopped;
			std::atomic_bool shouldPause;
			std::atomic_bool shouldStop;
			std::mutex _mutex;
			std::condition_variable _condition;
			std::function<Callable> _callable;
			bool hasDerivedClass=false; //是否有子类
			bool isLoop=false;
			bool isCallableSet=false;

			template<typename ...Args>
			void run(Args ...args) //非virtual类成员函数，其指针与通常函数指针不同
			{
				pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL); //允许退出线程
				pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL); //收到CANCEL信号后立即退出线程
				initialize();
				if(isLoop)
					while(!shouldStop)
					{
						_callable(args...); //isLoop=true时重复调用执行函数
						if(shouldPause)
						{
							std::unique_lock<std::mutex> locker(_mutex);
							while(shouldPause)
							{
								_condition.wait(locker); // Unlock _mutex and wait to be notified
							}
							locker.unlock();
						}
					}
				else
					_callable(args...); //isLoop=false时只调用一次执行函数
				shouldPause=false;
				shouldStop=false;
				terminate();
				exit(); //正常结束后释放线程，此时可通过start()再次调用而不必重新setThreadCallable()
			}

			bool hasDerived() //判断是否有子类
			{
				return typeid(Derived)!=typeid(void);
			}

		protected:
			void initialize() //执行函数执行之前调用一次
			{
				if(hasDerivedClass)
					(Derived*)this->initialize();
			}

			void terminate() //执行函数（包括isLoop=true时的情况）结束后调用一次
			{
				if(hasDerivedClass)
					(Derived*)this->terminate();
			}

			void before_stop() //每次成功调用stop()前调用一次
			{
				if(hasDerivedClass)
					(Derived*)this->before_stop();
			}

			void after_stop() //每次成功调用stop()后调用一次
			{
				if(hasDerivedClass)
					(Derived*)this->after_stop();
			}

			void before_suspended() //每次成功调用suspended()前调用一次
			{
				if(hasDerivedClass)
					(Derived*)this->before_suspended();
			}

			void after_suspended() //每次成功调用suspended()后调用一次
			{
				if(hasDerivedClass)
					(Derived*)this->after_suspended();
			}

			void before_resume() //每次成功调用resume()前调用一次
			{
				if(hasDerivedClass)
					(Derived*)this->before_resume();
			}

			void after_resume() //每次成功调用resume()后调用一次
			{
				if(hasDerivedClass)
					(Derived*)this->after_resume();
			}
		public:
			Thread()
			{
				shouldPause=false;
				shouldStop=false;
				hasDerivedClass=hasDerived();
			}
			//isLoop设定是否循环执行运行函数。如果设定为false，则不可使用pause()、resume()、stop()，在执行运行函数期间只可执行exit()操作
			Thread(Callable&& op,bool isLoop=false,ThreadWorkMode workState=Detach) :
					Thread() //默认采用Detach模式
			{
				setThreadCallable(op,isLoop);
				setThreadWorkMode(workState);
			}

			~Thread()
			{
				exit();
			}

			void exit() //调用exit()后强制结束线程并释放内存
			{
				pthread_cancel(_thread->native_handle());
				delete _thread;
				_thread=nullptr;
				_state=Stopped;
			}

			inline Thread& setThreadWorkMode(ThreadWorkMode workState) //只能在执行start()前调用，一旦执行start()后将无法改变
			{
				if(!_thread)
					_workState=workState;
				return *this;
			}

			Thread& setThreadCallable(Callable&& op,bool isLoop=false)
			{
				if(_thread) //如果线程已经存在，则强制结束线程并释放
					exit();
				_callable=op;
				isCallableSet=true;
				this->isLoop=isLoop;
				return *this;
			}

			Thread& clearThreadCallable() //清除已经设置的运行函数
			{
				if(_thread) //如果线程已经存在，则强制结束线程并释放
					exit();
				isCallableSet=false;
				return *this;
			}

			template<typename ...Args>
			void start(Args&&... args)
			{
				if((!isCallableSet)||_thread) //没有设置运行函数，或已经调用了start()且不是处于Stopped状态就直接返回
					return;
				_thread=new std::thread(std::bind(&Thread<Callable,Derived>::run<Args...>,this,args...),args...);
				_state=Running;
				switch(_workState)
				{
				case Join:
					_thread->join();
					break;
				case Detach:
					_thread->detach();
					break;
				}
			}
			//返回0表示操作成功，返回-1表示失败
			int suspend()
			{
				if(_thread&&isLoop)
				{
					before_suspended();
					shouldPause=true;
					_state=Suspended;
					after_suspended();
					return 0;
				}
				return -1;
			}

			int resume()
			{
				if(_thread&&isLoop)
				{
					before_resume();
					shouldPause=false;
					_condition.notify_all();
					_state=Running;
					after_resume();
					return 0;
				}
				return -1;
			}

			int stop() //stop()后依然可以直接继续调用start()重新执行
			{
				if(_thread&&isLoop)
				{
					before_stop();
					shouldPause=false;
					shouldStop=true;
					_condition.notify_all();
					_thread->join();
					exit();
					after_stop();
					return 0;
				}
				return -1;
			}

			inline ThreadWorkMode getThreadWorkState()
			{
				return _workState;
			}

			inline ThreadState getThreadState()
			{
				return _state;
			}

			inline std::thread::id getThreadID() //线程不存在则返回id(0)
			{
				return _thread?_thread->get_id():std::thread::id(0);
			}

			inline std::thread::native_handle_type getNativeHandle() //线程不存在则返回nullptr
			{
				return _thread?_thread->native_handle():(std::thread::native_handle_type)nullptr;
			}
		};
	}
}
#endif//ANDROMEDA_UTIL_THREAD