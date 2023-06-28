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
		class MainLoopThread:public andromeda::util::Thread<typename DerivedApp::MainLoopFunc,MainLoopThread<DerivedApp>>
		{
			friend class has_func(initialize)<void>;
			friend class has_func(terminate)<void>;
			friend class has_func(before_stop)<void>;
			friend class has_func(after_stop)<void>;
			friend class has_func(before_suspended)<void>;
			friend class has_func(after_suspended)<void>;
			friend class has_func(before_resume)<void>;
			friend class has_func(after_resume)<void>;
			friend class andromeda::util::Thread<typename DerivedApp::MainLoopFunc,MainLoopThread<DerivedApp>>;
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
					andromeda::util::Thread<typename DerivedApp::MainLoopFunc,MainLoopThread<DerivedApp>>(&(derived_app->_loop),&(derived_app->isRunning),andromeda::util::ThreadWorkMode::Detach)
			{
				app=derived_app;
			}

			using andromeda::util::Thread<typename DerivedApp::MainLoopFunc,MainLoopThread<DerivedApp>>::setThreadCallable;
			using andromeda::util::Thread<typename DerivedApp::MainLoopFunc,MainLoopThread<DerivedApp>>::setThreadWorkMode;
			using andromeda::util::Thread<typename DerivedApp::MainLoopFunc,MainLoopThread<DerivedApp>>::start;
			using andromeda::util::Thread<typename DerivedApp::MainLoopFunc,MainLoopThread<DerivedApp>>::stop;
		};
	}
}

#endif//ANDROMEDA_APP_MAINLOOPTHREAD
