#ifndef ANDROMEDA_APP_APPLICATION
#define ANDROMEDA_APP_APPLICATION

#include "../../lib/opengl/glad/glad.h"
#include "../../lib/opengl/GLFW/glfw3.h"
#include "Window.hpp"
#include "FrameRate.hpp"
#include "MainLoopThread.hpp"

namespace andromeda {
	namespace app {
		template<typename Derived>
		class Application
		{
		private:
			Window window;
			FrameRate frameRate;
			std::atomic_bool isRunning=false;
			MainLoopThread<Derived>* appMainLoopThread=nullptr;

			void _initialize() //�ڲ�ʹ�õĳ�ʼ������
			{

				initialize();
			}

			void _loop() //���߳�ִ�е����������˺�����ѭ��ִ��
			{
				if(isRunning)
				{
					glClear(GL_COLOR_BUFFER_BIT);
					update(frameRate.get_tpf());
					_render();
					glfwSwapBuffers(window);
					glfwPollEvents();
					frameRate.calc();
				}
				else
					appMainLoopThread.stop();
			}

			void _render()
			{

			}

			void _terminate() //�ڲ�ʹ�õ���ֹ����
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
				bool init_app=true; //�����Ҫ�Ŀ�û�м��أ��򲻳�ʼ�����࣬�޷�ʹ�ø���
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
					return;
				new (this) Window(window_title?window_title:"Andromeda Application");
				appMainLoopThread=new MainLoopThread<Derived>(this);
				appMainLoopThread.setThreadCallable(_loop,true);
			}

			void exit()
			{
				isRunning=false;
			}

			void launch(bool blocked=true) //blocked=trueʱ����ֱ��Application�߳�ִ����Ϻ��ִ��֮��Ĵ���
			{
				if(blocked)
					appMainLoopThread.setThreadWorkMode(andromeda::util::ThreadWorkMode::Join);
				isRunning=true;
				glfwMakeContextCurrent(window);
				appMainLoopThread.start(); //Application�����߳̿��������������blocked=true���������߳̾�����ֱ��isRunning=false
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
		}
		;
	}
}

#endif//ANDROMEDA_APP_APPLICATION
