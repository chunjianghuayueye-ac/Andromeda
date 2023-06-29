#ifndef ANDROMEDA_APP_MAINLOOPTHREAD
#define ANDROMEDA_APP_MAINLOOPTHREAD

#include "../util/Thread.hpp"
#include "Application.hpp"
#include "FrameRate.hpp"

namespace andromeda {
	namespace app {
		template<typename Derived>
		class Application;
		//��ѭ���̣߳�������Ⱦ���¼�����
		template<typename DerivedApp>
		class MainLoopThread:public andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>
		{
			friend class has_func(initialize)<void>;
			friend class has_func(run)<void>;
			friend class has_func(terminate)<void>;
			friend class has_func(before_stop)<void>;
			friend class has_func(after_stop)<void>;
			friend class has_func(before_suspended)<void>;
			friend class has_func(after_suspended)<void>;
			friend class has_func(before_resume)<void>;
			friend class has_func(after_resume)<void>;
			friend class andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>;
			friend class andromeda::app::Application<DerivedApp>;
		protected:
			DerivedApp* app;
			FrameRate frameRate;

			void initialize()
			{
				frameRate.init();
			}

			void run() //���߳�ִ�е���Ⱦ����
			{
				app->update(frameRate.get_tpf());
				frameRate.calc();
			}

			void terminate()
			{

			}
		public:
			MainLoopThread(DerivedApp* derived_app) :
					andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>(&(derived_app->isRunning),andromeda::util::ThreadWorkMode::Detach)
			{
				app=derived_app;
			}

			inline void setFPSLimit(int render_fps)
			{
				frameRate.set_fps_limit(render_fps);
			}

			inline int getFPS()
			{
				return frameRate.get_fps();
			}

			inline int getFPSCount()
			{
				return frameRate.get_fps_count();
			}

			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>::start;
			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>::stop;
		};
	}
}

#endif//ANDROMEDA_APP_MAINLOOPTHREAD
