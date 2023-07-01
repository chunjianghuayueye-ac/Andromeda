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

/* OpenGL与glfw部分函数必须在主线程调用，例如glfwCreateWindow()和glfwPollEvents()，因此更新（主）循环和渲染循环分开
 * 应用每秒更新数为帧率fps，渲染帧率是每秒渲染的次数，和帧率可以不相等，是实际画面的刷新率。两者可以设定一致（帧率同步，synchronize_fps=true）
 * 渲染、事件系统为主线程执行，在程序启动初期的初始化操作也由主线程执行
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
			//子类必须添加如下friend class
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
			FrameRate renderFrameRate;		//渲染循环计数器
			bool isRunning=false;
			MainLoopThread<Derived>* mainLoopThread=nullptr;
			andromeda::util::ThreadTurn threadTurn;
			//主线程函数，负责事件处理和更新
			//内部使用的初始化函数
			void _initialize(const char* window_title=nullptr,int width=800,int height=600,bool isfullscreen=false,andromeda::image::color::ColorRGBA backColor_={0,0,0,0},GLFWmonitor* monitor_=glfwGetPrimaryMonitor())
			{
				isRunning=true;
				preinitialize();		//可以调用glfwWindowHint()
				glfwWindowHint(GLFW_DOUBLEBUFFER,GLFW_FALSE);
				new (this) Window(window_title?window_title:"Andromeda Application",width,height,isfullscreen,backColor_,monitor_); //初始化window
				glfwSetFramebufferSizeCallback(window,_glfw_framebuffer_size_callback);
				glfwMakeContextCurrent(window);
				if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //GLAD的加载要在glfwMakeContextCurrent()之后进行
					PRINT_MESSAGE("Initialize GLAD failed.")

				initialize();
				glClearColor(window.getBackColor(andromeda::image::ColorChannel::R),window.getBackColor(andromeda::image::ColorChannel::G),window.getBackColor(andromeda::image::ColorChannel::B),window.getBackColor(andromeda::image::ColorChannel::A));
				glViewport(0,0,window.getWidth(),window.getHeight());
			}

			void _terminate() //内部使用的终止函数
			{
				terminate();
			}

			inline void turnMainLoopThread()
			{
				threadTurn.turn(*mainLoopThread);
			}

		protected:
			void initialize() //在各系统初始化完成后调用
			{
				if(andromeda::tmp::is_class<Derived>::result&&has_func(initialize)<void>::check<Derived>::result)
					((Derived*)this)->initialize();
			}
			void preinitialize() //在各系统初始化之前调用
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
				mainLoopThread=new MainLoopThread<Derived>((Derived*)this);
				synchronize_fps=true;//默认开启帧率同步
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
					//输入处理
					glfwPollEvents();
					if(synchronize_fps)
						turnMainLoopThread();
					//渲染
					glClear(GL_COLOR_BUFFER_BIT);
					if(andromeda::tmp::is_class<Derived>::result&&has_func(render_update)<void,float>::check<Derived>::result)//如果子类没有render_update()则此调用将优化掉
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

			inline int getRenderFPSCount() //获取当前所在帧
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

			inline int getFPSCount() //获取当前所在帧
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