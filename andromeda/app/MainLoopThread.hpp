#include "../util/Thread.hpp"

namespace andromeda {
	namespace app {
		//��ѭ���̣߳�������Ⱦ���¼�����
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
