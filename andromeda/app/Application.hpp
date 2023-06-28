#ifndef ANDROMEDA_APP_APPLICATION
#define ANDROMEDA_APP_APPLICATION

#include "../macros/Debug.h"
#include "../../lib/opengl/glad/glad.h"
#include "../../lib/opengl/GLFW/glfw3.h"
#include "Window.hpp"
#include "FrameRate.hpp"
#include "MainLoopThread.hpp"
#include "../image/ColorChannel.hpp"
#include "../tmp/Types.h"

#ifndef HAS_FUNC_INITIALIZE
#define HAS_FUNC_INITIALIZE
def_cls_has_func(initialize)
#endif//#HAS_FUNC_INITIALIZE#ifndef HAS_FUNC_UPDATE
#define HAS_FUNC_UPDATE
def_cls_has_func(update)
#endif//HAS_FUNC_UPDATE#ifndef HAS_FUNC_TERMINATE
#define HAS_FUNC_TERMINATE
def_cls_has_func(terminate)
#endif//HAS_FUNC_TERMINATEnamespace andromeda {
	extern bool use_opengl;
	extern bool use_portaudio;

	namespace app {
		template<typename Derived>
		class Application
		{
			//��������������friend class
			//start
			friend class has_func(initialize)<void>;
			friend class has_func(terminate)<void>;
			friend class has_func(update)<void,float>;
			//end
			friend class MainLoopThread<Derived>;
		private:
			Window window;
			FrameRate frameRate;
			bool isRunning=false;
			MainLoopThread<Derived>* appMainLoopThread=nullptr;

			void _initialize() //�ڲ�ʹ�õĳ�ʼ����������appMainLoopThread���߳̿�ʼִ��
			{
				isRunning=true;
				glfwMakeContextCurrent(window);
				if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //GLAD�ļ���Ҫ��glfwMakeContextCurrent()֮�����
					PRINT_MESSAGE("Initialize GLAD failed.")
				glViewport(0,0,window.getWidth(),window.getHeight());
				initialize();
				frameRate.init();
			}

			void _loop() //���߳�ִ�е�������������_initialize()ִ�У��˺�������appMainLoopThreadѭ��ִ��
			{
				glClearColor(window.getBackColor(andromeda::image::ColorChannel::R),window.getBackColor(andromeda::image::ColorChannel::G),window.getBackColor(andromeda::image::ColorChannel::B),window.getBackColor(andromeda::image::ColorChannel::A));
				glClear(GL_COLOR_BUFFER_BIT);
				update(frameRate.get_tpf());
				_render();
				glfwSwapBuffers(window);
				glfwPollEvents();
				frameRate.calc();
				if(glfwWindowShouldClose(window))
					isRunning=false;
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
				if(andromeda::tmp::is_class<Derived>::result&&has_func(initialize)<void>::check<Derived>::result)
					((Derived*)this)->initialize();
			}
			void terminate()
			{
				if(andromeda::tmp::is_class<Derived>::result&&has_func(terminate)<void>::check<Derived>::result)
					((Derived*)this)->terminate();
			}
			void update(float tpf)
			{
				if(andromeda::tmp::is_class<Derived>::result&&has_func(update)<void,float>::check<Derived>::result)
					((Derived*)this)->update(tpf);
			}

		public:
			typedef decltype(&Application<Derived>::_loop) MainLoopFunc;

			Application(const char* window_title=nullptr,int width=800,int height=600,bool isfullscreen=false,andromeda::image::color::ColorRGBA backColor_={0,0,0,0},GLFWmonitor* monitor_=glfwGetPrimaryMonitor())
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
				{
					PRINT_MESSAGE("Application not initialized.")
					return;
				}
				new (this) Window(window_title?window_title:"Andromeda Application",width,height,isfullscreen,backColor_,monitor_); //��ʼ��window
				appMainLoopThread=new MainLoopThread<Derived>((Derived*)this);
			}

			void exit()
			{
				isRunning=false;
			}

			void launch(bool blocked=true) //blocked=trueʱ����ֱ��Application�߳�ִ����Ϻ��ִ��֮��Ĵ���
			{
				if(blocked)
					appMainLoopThread->setThreadWorkMode(andromeda::util::ThreadWorkMode::Join);
				appMainLoopThread->start(this); //Application�����߳̿��������������blocked=true���������߳̾�����ֱ��isRunning=false
			}

			inline int getFPS()
			{
				return frameRate.get_fps();
			}

			inline void setFPSLimit(int fps_limit)
			{
				frameRate.set_fps_limit(fps_limit);
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