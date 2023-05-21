#include "../util/Thread.hpp"

namespace andromeda {
	namespace app {
		//主循环线程，负责渲染和事件处理
		template<typename DerivedApp>
		class MainLoopThread:public andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>
		{
		protected:
			DerivedApp* app;

			void initialize()
			{
				app->_initialize();
				app->frameRate.init();
			}

			void terminate()
			{
				app->_terminate();
			}
		public:
			MainLoopThread(DerivedApp* derived_app)
			{
				app=derived_app;
			}

			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>::setThreadCallable;
			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>::setThreadWorkMode;
			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>::start;
		};
	}
}
