#ifndef ANDROMEDA_APP_APPLICATION
#define ANDROMEDA_APP_APPLICATION

#include "../../lib/opengl/glad/glad.h"
#include "../../lib/opengl/GLFW/glfw3.h"
#include "Window.hpp"
#include "FrameRate.hpp"

namespace andromeda {
	namespace app {
		template<typename Derived>
		class Application
		{
		private:
			Window window;
			FrameRate frameRate;
			bool isRunning=false;

			void _initialize() //内部使用的初始化函数
			{

				initialize();
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
				(Derived*)this->initialize();
			}
			void terminate()
			{
				(Derived*)this->terminate();
			}
			void update(float tpf)
			{
				(Derived*)this->update(tpf);
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
					PRINT_MESSAGE("OpenGL is not used. Please set andromeda::use_portaudio to true.")
					init_app=false;
				}
				if(!init_app)
					return;
				new (this) Window(window_title?window_title:"Andromeda Application");
			}

			void launch()
			{
				isRunning=true;
				_initialize();
				glfwMakeContextCurrent(window);
				frameRate.init();
				while(isRunning)
				{
					glClear(GL_COLOR_BUFFER_BIT);
					update(frameRate.get_tpf());
					_render();
					glfwSwapBuffers(window);
					glfwPollEvents();
					frameRate.calc();
				}
				_terminate();
			}

			void exit()
			{
				isRunning=false;
			}

			void launchAsync();

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
