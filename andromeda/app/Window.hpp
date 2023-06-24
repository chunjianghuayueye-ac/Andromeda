#ifndef ANDROMEDA_APP_WINDOW
#define ANDROMEDA_APP_WINDOW

#include "../../lib/opengl/glad/glad.h"
#include "../../lib/opengl/GLFW/glfw3.h"

namespace andromeda {
	namespace app {
		class Window
		{
		private:
			GLFWwindow* windowID;
			GLFWmonitor* monitor;
			int width,height;
			bool isFullScreen;
			const char* title;

		public:
			Window();
			Window(int width,int height,const char* title,bool isfullscreen,GLFWmonitor* monitor_=glfwGetPrimaryMonitor());
			Window(int width,int height,const char* title);
			Window(const char* title);

			inline operator GLFWwindow*()
			{
				return windowID;
			}

			inline int getWidth(void)
			{
				return width;
			}

			inline int getHeight(void)
			{
				return height;
			}

			inline bool isWindowFullScreen(void)
			{
				return isFullScreen;
			}

			inline void setFullScreen(bool isFullScreen_)
			{
				isFullScreen=isFullScreen_;
				glfwSetWindowMonitor(windowID,isFullScreen_?monitor:nullptr,0,0,width,height,GLFW_DONT_CARE);
			}

			inline void setWindowedFullScreen(bool isFullScreen_)
			{
				const GLFWvidmode* mode=glfwGetVideoMode(monitor);
				glfwSetWindowMonitor(windowID,monitor,0,0,mode->width,mode->height,mode->refreshRate);
			}

			inline void setWindowSize(int width_,int height_)
			{
				width=width_;
				height=height_;
				glfwSetWindowSize(windowID,width_,height_);
			}

			inline bool setIsAlwaysOnTop(bool floating) //设置窗口是否总是置顶
			{
				glfwSetWindowAttrib(windowID,GLFW_FLOATING,floating);
				return glfwGetWindowAttrib(windowID,GLFW_FLOATING); //设置成功则返回true
			}

			inline bool setWindowFramebufferTransparent(bool transparent=false) //设置窗口渲染的背景是否透明。注意即使内容透明，事件依旧不会穿透窗口透明部分，它们会被窗口捕获
			{
				glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,transparent);
				return glfwGetWindowAttrib(windowID,GLFW_TRANSPARENT_FRAMEBUFFER); //设置成功则返回true
			}

			inline float setWindowOpacity(float opacity=1) //窗口整体的透明度设置，位于0-1间
			{
				glfwSetWindowOpacity(windowID,opacity);
				return glfwGetWindowOpacity(windowID); //返回当前的窗口透明度。如果设置成功则应该返回opacity
			}
		};
	}
}

#endif//ANDROMEDA_APP_WINDOW
