#ifndef ANDROMEDA_APP_APPLICATION
#define ANDROMEDA_APP_APPLICATION

#include "../macros/Debug.h"
#include "../../lib/opengl/glad/glad.h"
#include "../../lib/opengl/GLFW/glfw3.h"
#include "Window.hpp"
#include "FrameRate.hpp"
#include "MainLoopThread.hpp"

namespace andromeda {
	extern bool use_opengl;
	extern bool use_portaudio;

	namespace app {
		template<typename Derived>
		class Application
		{
			friend class MainLoopThread<Derived>;
		private:
			Window window;
			FrameRate frameRate;
			bool isRunning=false;
			MainLoopThread<Derived>* appMainLoopThread=nullptr;

			void _initialize() //内部使用的初始化函数，由appMainLoopThread在线程开始执行
			{
				isRunning=true;
				glfwMakeContextCurrent(window);
				initialize();
				frameRate.init();
			}

			void _loop() //多线程执行的主函数，紧跟_initialize()执行，此函数将由appMainLoopThread循环执行
			{
				glClear(GL_COLOR_BUFFER_BIT);
				update(frameRate.get_tpf());
				_render();
				glfwSwapBuffers(window);
				glfwPollEvents();
				frameRate.calc();
			}

			void _render()
			{

			}

			void _terminate() //内部使用的终止函数
			{
				terminate();
			}
		protected:
			void initialize()
			{
				((Derived*)this)->initialize();
			}
			void terminate()
			{
				((Derived*)this)->terminate();
			}
			void update(float tpf)
			{
				((Derived*)this)->update(tpf);
			}

		public:
			Application(const char* window_title=nullptr)
			{
				bool init_app=true; //如果需要的库没有加载，则不初始化该类，无法使用该类
				if(!andromeda::use_opengl)
				{
					PRINT_MESSAGE("OpenGL is not used. Please set andromeda::use_opengl to true.")
					init_app=false;
				}
				if(!andromeda::use_portaudio)
				{
					PRINT_MESSAGE("PortAudio is not used. Please set andromeda::use_portaudio to true.")
					init_app=false;
				}
				if(!init_app)
				{
					PRINT_MESSAGE("Application not initialized.")
					return;
				}
				new (this) Window(window_title?window_title:"Andromeda Application"); //初始化window
				appMainLoopThread=new MainLoopThread<Derived>((Derived*)this);
			}

			void exit()
			{
				isRunning=false;
			}

			void launch(bool blocked=true) //blocked=true时，则直到Application线程执行完毕后才执行之后的代码
			{
				if(blocked)
					appMainLoopThread->setThreadWorkMode(andromeda::util::ThreadWorkMode::Join);
				appMainLoopThread->start(); //Application的主线程开启后，如果设置了blocked=true则程序的主线程就休眠直到isRunning=false
			}

			inline int getFPS()
			{
				return frameRate.get_fps();
			}

			inline int getWindowHeight(void)
			{
				return window.getHeight();
			}

			inline int getWindowWidth(void)
			{
				return window.getWidth();
			}

			inline bool isFullScreen(void)
			{
				return window.isWindowFullScreen();
			}

			inline void setFullScreen(bool isFullScreen)
			{
				window.setFullScreen(isFullScreen);
			}

			inline void setWindowSize(int width,int height)
			{
				window.setWindowSize(width,height);
			}
		};
	}
}

#endif//ANDROMEDA_APP_APPLICATION