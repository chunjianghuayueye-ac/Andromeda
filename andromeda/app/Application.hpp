#ifndef ANDROMEDA_APP_APPLICATION
#define ANDROMEDA_APP_APPLICATION

#include "../macros/Debug.h"
#include "../../lib/opengl/glad/glad.h"
#include "../../lib/opengl/GLFW/glfw3.h"
#include "Window.hpp"
#include "FrameRate.hpp"
#include "MainLoopThread.hpp"
#include "../util/ThreadTurn.hpp"
#include "../image/ColorChannel.hpp"
#include "../tmp/Types.h"

/* OpenGL��glfw���ֺ������������̵߳��ã�����glfwCreateWindow()��glfwPollEvents()����˸��£�����ѭ������Ⱦѭ���ֿ�
 * Ӧ��ÿ�������Ϊ֡��fps����Ⱦ֡����ÿ����Ⱦ�Ĵ�������֡�ʿ��Բ���ȣ���ʵ�ʻ����ˢ���ʡ����߿����趨һ�£�֡��ͬ����synchronize_fps=true��
 * ��Ⱦ���¼�ϵͳΪ���߳�ִ�У��ڳ����������ڵĳ�ʼ������Ҳ�����߳�ִ��
 */

#ifndef HAS_FUNC_INITIALIZE
#define HAS_FUNC_INITIALIZE
def_cls_has_func(initialize)
#endif//#HAS_FUNC_INITIALIZE#ifndef HAS_FUNC_PREINITIALIZE
#define HAS_FUNC_PREINITIALIZE
def_cls_has_func(preinitialize)
#endif//#HAS_FUNC_PREINITIALIZE#ifndef HAS_FUNC_UPDATE#define HAS_FUNC_UPDATEdef_cls_has_func(update)#endif//HAS_FUNC_UPDATE
#ifndef HAS_FUNC_RENDER_UPDATE
#define HAS_FUNC_RENDER_UPDATE
def_cls_has_func(render_update)
#endif//#HAS_FUNC_RENDER_UPDATE#ifndef HAS_FUNC_TERMINATE#define HAS_FUNC_TERMINATEdef_cls_has_func(terminate)#endif//HAS_FUNC_TERMINATEnamespace andromeda {	extern bool use_opengl;	extern bool use_portaudio;	extern void _glfw_framebuffer_size_callback(GLFWwindow* window,int width,int height);

	namespace app {
		template<typename Derived>
		class Application
		{
			//��������������friend class
			//start
			friend class has_func(initialize)<void>;
			friend class has_func(preinitialize)<void>;
			friend class has_func(terminate)<void>;
			friend class has_func(update)<void,float>;
			friend class has_func(render_update)<void,float>;
			//end
			friend class MainLoopThread<Derived>;
		private:
			Window window;
			FrameRate renderFrameRate;		//��Ⱦѭ��������
			bool isRunning=false;
			MainLoopThread<Derived>* mainLoopThread=nullptr;
			andromeda::util::ThreadTurn threadTurn;
			//���̺߳����������¼�����͸���
			//�ڲ�ʹ�õĳ�ʼ������
			void _initialize(const char* window_title=nullptr,int width=800,int height=600,bool isfullscreen=false,andromeda::image::color::ColorRGBA backColor_={0,0,0,0},GLFWmonitor* monitor_=glfwGetPrimaryMonitor())
			{
				isRunning=true;
				preinitialize();		//���Ե���glfwWindowHint()
				glfwWindowHint(GLFW_DOUBLEBUFFER,GLFW_FALSE);
				new (this) Window(window_title?window_title:"Andromeda Application",width,height,isfullscreen,backColor_,monitor_); //��ʼ��window
				glfwSetFramebufferSizeCallback(window,_glfw_framebuffer_size_callback);
				glfwMakeContextCurrent(window);
				if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //GLAD�ļ���Ҫ��glfwMakeContextCurrent()֮�����
					PRINT_MESSAGE("Initialize GLAD failed.")

				initialize();
				glClearColor(window.getBackColor(andromeda::image::ColorChannel::R),window.getBackColor(andromeda::image::ColorChannel::G),window.getBackColor(andromeda::image::ColorChannel::B),window.getBackColor(andromeda::image::ColorChannel::A));
				glViewport(0,0,window.getWidth(),window.getHeight());
			}

			void _terminate() //�ڲ�ʹ�õ���ֹ����
			{
				terminate();
			}

			inline void turnMainLoopThread()
			{
				threadTurn.turn(*mainLoopThread);
			}

		protected:
			void initialize() //�ڸ�ϵͳ��ʼ����ɺ����
			{
				if(andromeda::tmp::is_class<Derived>::result&&has_func(initialize)<void>::check<Derived>::result)
					((Derived*)this)->initialize();
			}
			void preinitialize() //�ڸ�ϵͳ��ʼ��֮ǰ����
			{
				if(andromeda::tmp::is_class<Derived>::result&&has_func(preinitialize)<void>::check<Derived>::result)
					((Derived*)this)->preinitialize();
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

			void render_update(float render_tpf)
			{
				if(andromeda::tmp::is_class<Derived>::result&&has_func(render_update)<void,float>::check<Derived>::result)
					((Derived*)this)->render_update(render_tpf);
			}

		public:
			std::atomic_bool synchronize_fps;

			inline operator andromeda::util::ThreadTurn*()
			{
				return &threadTurn;
			}

			Application()
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
				mainLoopThread=new MainLoopThread<Derived>((Derived*)this);
				synchronize_fps=true;//Ĭ�Ͽ���֡��ͬ��
			}

			void exit()
			{
				isRunning=false;
			}

			void launch(const char* window_title=nullptr,int width=800,int height=600,bool isfullscreen=false,andromeda::image::color::ColorRGBA backColor_={0,0,0,0},GLFWmonitor* monitor_=glfwGetPrimaryMonitor())
			{
				_initialize(window_title,width,height,isfullscreen,backColor_,monitor_);
				mainLoopThread->start();
				renderFrameRate.init();
				while(isRunning)
				{
					//���봦��
					glfwPollEvents();
					if(synchronize_fps)
						turnMainLoopThread();
					//��Ⱦ
					glClear(GL_COLOR_BUFFER_BIT);
					if(andromeda::tmp::is_class<Derived>::result&&has_func(render_update)<void,float>::check<Derived>::result)//�������û��render_update()��˵��ý��Ż���
						render_update(renderFrameRate.get_tpf());
					glFlush();
					renderFrameRate.calc();
					if(glfwWindowShouldClose(window))
						isRunning=false;
				}
				_terminate();
			}

			inline int getRenderFPS()
			{
				return renderFrameRate.get_fps();
			}

			inline int getRenderFPSCount() //��ȡ��ǰ����֡
			{
				return renderFrameRate.get_fps_count();
			}

			inline void setRenderFPSLimit(int fps_limit)
			{
				renderFrameRate.set_fps_limit(fps_limit);
			}

			inline int getFPS()
			{
				return mainLoopThread->getFPS();
			}

			inline int getFPSCount() //��ȡ��ǰ����֡
			{
				return mainLoopThread->getFPSCount();
			}

			inline void setFPSLimit(int fps_limit)
			{
				mainLoopThread->setFPSLimit(fps_limit);
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

			inline void setBackColor(andromeda::image::color::ColorRGBA color)
			{
				window.setBackColor(color);
			}
		};
	}
}

#endif//ANDROMEDA_APP_APPLICATION