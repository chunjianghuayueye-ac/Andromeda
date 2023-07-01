#ifndef ANDROMEDA_APP_MAINLOOPTHREAD
#define ANDROMEDA_APP_MAINLOOPTHREAD

#include "../util/Thread.hpp"
#include "../util/ThreadTurn.hpp"
#include "Application.hpp"
#include "FrameRate.hpp"

namespace andromeda {
	namespace app {
		template<typename Derived>
		class Application;
		//主循环线程，负责渲染和事件处理
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
			andromeda::util::ThreadTurn threadTurn;

			void initialize()
			{
				frameRate.init();
			}

			void run() //多线程执行的渲染函数
			{
				app->update(frameRate.get_tpf());
				if(app->synchronize_fps)
					turnRenderThread();
				frameRate.calc();
			}

			void terminate()
			{

			}

			inline void turnRenderThread()
			{
				threadTurn.turn(*app);
			}
		public:
			inline operator andromeda::util::ThreadTurn*()
			{
				return &threadTurn;
			}

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
