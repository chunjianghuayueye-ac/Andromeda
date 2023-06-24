#ifndef ANDROMEDA_UTIL_THREAD
#define ANDROMEDA_UTIL_THREAD

#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "../tmp/Types.h"

//THREAD��������κ�Callable����������func�������ܴ���ClassObj.Func����Ϊdecltype�޷��������ܵ����غ���
#define THREAD(obj_name,func) andromeda::util::Thread<decltype(func)> obj_name(func)

namespace andromeda {
	namespace util {
		enum ThreadWorkMode
		{
			Join, Detach
		};
		//����Stoppedʱ_thread=nullptr��ֻ�����µ���start()ִ��
		enum ThreadState
		{
			Stopped, Running, Suspended
		};

		template<typename Thread>
		static inline void exitThread(Thread* thread)
		{
			thread->exit();
		}

		template<typename Callable,typename Derived=void> //CallableΪ�κοɵ��ö��󣨰�����Ա��������Derived���ڼ̳�ʱ��������CRTP����is_class<Derived>=false���ʾ���̳�
		class Thread
		{
		public:
			typedef typename andromeda::tmp::degenerate_func<Callable>::result_type DegeneratedCallableType;
		private:
			friend void exitThread<>(Thread<Callable,Derived>* thread);

			std::thread* _thread=nullptr;
			ThreadWorkMode _workState=Detach;
			ThreadState _state=Stopped;
			std::atomic_bool shouldPause;
			std::atomic_bool shouldStop;
			std::mutex _mutex;
			std::condition_variable _condition;
			void* _callable_obj; //����ԭʼ�ĵ��ö������ָ��
			std::function<DegeneratedCallableType> _callable; //����ʵ��ִ�еķ�װ��Ŀɵ��ö���Callable����ǳ�Ա�������˻�Ϊ��Ч����ͨ��������this
			bool* loopFlag=nullptr; //ѭ�����Ʊ�����nullptr��ʾ��ѭ��ִ��
			bool isCallableSet=false;
			template<typename ...Args>
			void run(Args ...args) //��virtual���Ա��������ָ����ͨ������ָ�벻ͬ
			{
				pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL); //�����˳��߳�
				pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL); //�յ�CANCEL�źź������˳��߳�
				initialize();
				if(isLoop())
					while(!shouldStop)
					{
						_callable(args...); //isLoop=trueʱ�ظ�����ִ�к���
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
					_callable(args...); //isLoop=falseʱֻ����һ��ִ�к���
				shouldPause=false;
				shouldStop=false;
				terminate();
				exit(); //�����������ͷ��̣߳���ʱ��ͨ��start()�ٴε��ö���������setThreadCallable()
			}

		protected:
			inline bool isLoop()
			{
				return loopFlag&&(*loopFlag);
			}
			//CRTPʵ��
			void initialize() //ִ�к���ִ��֮ǰ����һ��
			{
				if(andromeda::tmp::is_class<Derived>::result)
					((typename andromeda::tmp::_if<andromeda::tmp::is_class<Derived>::result,Derived,Thread<Callable,Derived>>::result_type*)this)->initialize();
			}

			void terminate() //ִ�к���������isLoop=trueʱ����������������һ��
			{
				if(andromeda::tmp::is_class<Derived>::result)
					((typename andromeda::tmp::_if<andromeda::tmp::is_class<Derived>::result,Derived,Thread<Callable,Derived>>::result_type*)this)->terminate();
			}

			void before_stop() //ÿ�γɹ�����stop()ǰ����һ��
			{
				if(andromeda::tmp::is_class<Derived>::result)
					((typename andromeda::tmp::_if<andromeda::tmp::is_class<Derived>::result,Derived,Thread<Callable,Derived>>::result_type*)this)->before_stop();
			}

			void after_stop() //ÿ�γɹ�����stop()�����һ��
			{
				if(andromeda::tmp::is_class<Derived>::result)
					((typename andromeda::tmp::_if<andromeda::tmp::is_class<Derived>::result,Derived,Thread<Callable,Derived>>::result_type*)this)->after_stop();
			}

			void before_suspended() //ÿ�γɹ�����suspended()ǰ����һ��
			{
				if(andromeda::tmp::is_class<Derived>::result)
					((typename andromeda::tmp::_if<andromeda::tmp::is_class<Derived>::result,Derived,Thread<Callable,Derived>>::result_type*)this)->before_suspended();
			}

			void after_suspended() //ÿ�γɹ�����suspended()�����һ��
			{
				if(andromeda::tmp::is_class<Derived>::result)
					((typename andromeda::tmp::_if<andromeda::tmp::is_class<Derived>::result,Derived,Thread<Callable,Derived>>::result_type*)this)->after_suspended();
			}

			void before_resume() //ÿ�γɹ�����resume()ǰ����һ��
			{
				if(andromeda::tmp::is_class<Derived>::result)
					((typename andromeda::tmp::_if<andromeda::tmp::is_class<Derived>::result,Derived,Thread<Callable,Derived>>::result_type*)this)->before_resume();
			}

			void after_resume() //ÿ�γɹ�����resume()�����һ��
			{
				if(andromeda::tmp::is_class<Derived>::result)
					((typename andromeda::tmp::_if<andromeda::tmp::is_class<Derived>::result,Derived,Thread<Callable,Derived>>::result_type*)this)->after_resume();
			}
		public:
			Thread()
			{
				shouldPause=false;
				shouldStop=false;
			}
			//isLoop�趨�Ƿ�ѭ��ִ�����к���������趨Ϊfalse���򲻿�ʹ��pause()��resume()��stop()����ִ�����к����ڼ�ֻ��ִ��exit()����
			template<typename MCallable=Callable>
			Thread(MCallable& op,bool* loopFlag=nullptr,ThreadWorkMode workState=Detach) :
					Thread() //Ĭ�ϲ���Detachģʽ
			{
				setThreadCallable<MCallable>(op,loopFlag);
				setThreadWorkMode(workState);
			}
			template<typename MCallable=Callable>
			Thread(MCallable&& op,bool* loopFlag=nullptr,ThreadWorkMode workState=Detach) :
					Thread() //Ĭ�ϲ���Detachģʽ
			{
				setThreadCallable<MCallable>(op,loopFlag);
				setThreadWorkMode(workState);
			}

			~Thread()
			{
				exit();
			}

			void exit() //����exit()��ǿ�ƽ����̲߳��ͷ��ڴ�
			{
				pthread_cancel(_thread->native_handle());
				delete _thread;
				_thread=nullptr;
				_state=Stopped;
			}

			inline Thread& setThreadWorkMode(ThreadWorkMode workState) //ֻ����ִ��start()ǰ���ã�һ��ִ��start()���޷��ı�
			{
				if(!_thread)
					_workState=workState;
				return *this;
			}

			template<typename MCallable=Callable> //MCallable������Callable��ͬ������Callable������ͨ�������ͣ�MCallable�����Ч��Ա�������ͻ�������Ч�ɵ������ͣ�������ñ���һ��
			Thread& setThreadCallable(MCallable& op,bool* loopFlag=nullptr) //����_callable_objԭʼ���󣬶�ʵ��ִ�е�_callable������start()�вŻ�ʵ����
			{
				if(_thread) //����߳��Ѿ����ڣ���ǿ�ƽ����̲߳��ͷ�
					exit();
				_callable_obj=(void*)&op;
				isCallableSet=true;
				this->loopFlag=loopFlag;
				return *this;
			}

			template<typename MCallable=Callable>
			Thread& setThreadCallable(MCallable&& op,bool* loopFlag=nullptr)
			{
				if(_thread) //����߳��Ѿ����ڣ���ǿ�ƽ����̲߳��ͷ�
					exit();
				_callable_obj=(void*)&std::move(op);
				isCallableSet=true;
				this->loopFlag=loopFlag;
				return *this;
			}

			Thread& clearThreadCallable() //����Ѿ����õ����к���
			{
				if(_thread) //����߳��Ѿ����ڣ���ǿ�ƽ����̲߳��ͷ�
					exit();
				isCallableSet=false;
				return *this;
			}

			template<typename ...Args>
			void start(Args&&... args) //�ǳ�Ա��������
			{
				if((!isCallableSet)||_thread) //û���������к��������Ѿ�������start()�Ҳ��Ǵ���Stopped״̬��ֱ�ӷ���
					return;
				_callable=std::function<Callable>(*(Callable*)_callable_obj);
				_thread=new std::thread(std::bind(&Thread<Callable,Derived>::run<Args...>,this,args...));
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

			template<typename Class,typename ...Args>
			void start(Class* cls,Args&&... args) //��Ա��������
			{
				if((!isCallableSet)||_thread) //û���������к��������Ѿ�������start()�Ҳ��Ǵ���Stopped״̬��ֱ�ӷ���
					return;
				typedef typename andromeda::tmp::get_func_ret_type<Callable,Args...>::result_type ret_type;
				_callable=andromeda::tmp::bind_this<Class,ret_type,Args...>::bind(*(typename andromeda::tmp::func_type<Class,ret_type,Args...>::result_type*)_callable_obj,cls); //Ϊ��Ա������this
				_thread=new std::thread(std::bind(&Thread<Callable,Derived>::run<Args...>,this,args...));
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

			//����0��ʾ�����ɹ�������-1��ʾʧ��
			int suspend()
			{
				if(_thread&&isLoop())
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
				if(_thread&&isLoop())
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

			int stop() //stop()����Ȼ����ֱ�Ӽ�������start()����ִ��
			{
				if(_thread&&isLoop())
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

			inline std::thread::id getThreadID() //�̲߳������򷵻�id(0)
			{
				return _thread?_thread->get_id():std::thread::id(0);
			}

			inline std::thread::native_handle_type getNativeHandle() //�̲߳������򷵻�nullptr
			{
				return _thread?_thread->native_handle():(std::thread::native_handle_type)nullptr;
			}
		};
	}
}
#endif//ANDROMEDA_UTIL_THREAD