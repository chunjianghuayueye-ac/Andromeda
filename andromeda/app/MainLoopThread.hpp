#ifndef ANDROMEDA_APP_MAINLOOPTHREAD
#define ANDROMEDA_APP_MAINLOOPTHREAD

#include "../util/Thread.hpp"
#include "Application.hpp"

namespace andromeda {
	namespace app {
		template<typename Derived>
		class Application;
		//主循环线程，负责渲染和事件处理
		template<typename DerivedApp>
		class MainLoopThread:public andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>
		{
			friend class andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>;
		protected:
			DerivedApp* app;

			void initialize()
			{
				app->_initialize();
			}

			void terminate()
			{
				app->_terminate();
			}
		public:
			MainLoopThread(DerivedApp* derived_app) :
					andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>(&(derived_app->_loop),&(derived_app->isRunning),andromeda::util::ThreadWorkMode::Detach)
			{
				app=derived_app;
			}

			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>::setThreadCallable;
			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>::setThreadWorkMode;
			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>::start;
			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>::stop;
		};
	}
}

#endif//ANDROMEDA_APP_MAINLOOPTHREAD
