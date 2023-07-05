#ifndef ANDROMEDA_APP_WINDOW
#define ANDROMEDA_APP_WINDOW

#include "../../lib/opengl/glad/glad.h"
#include "../../lib/opengl/GLFW/glfw3.h"
#include "../image/ColorRGBA.hpp"
#include "../image/ColorChannel.hpp"

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
			andromeda::image::ColorRGBA backColor;

		public:
			Window()=default; //该构造函数不会实际初始化窗口仅分配内存，需要重新调用其他构造函数以正常工作
			Window(const char* title,int width=800,int height=600,andromeda::image::ColorRGBA backColor_={0,0,0,0},bool isfullscreen=false,GLFWmonitor* monitor_=glfwGetPrimaryMonitor());

			inline operator GLFWwindow*()
			{
				return windowID;
			}

			inline void setBackColor(andromeda::image::ColorRGBA backColor_={0,0,0,0})
			{
				backColor=backColor_;
			}

			inline andromeda::image::ColorRGBA getBackColor()
			{
				return backColor;
			}

			inline Window& setBackColor(andromeda::image::ColorChannel ch,float v)
			{
				backColor.setRGBA(ch,v);
				return *this;
			}

			inline float getBackColor(andromeda::image::ColorChannel ch)
			{
				return backColor.getRGBA(ch);
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

			inline Window& setFullScreen(bool isFullScreen_)
			{
				isFullScreen=isFullScreen_;
				glfwSetWindowMonitor(windowID,isFullScreen_?monitor:nullptr,0,0,width,height,GLFW_DONT_CARE);
				return *this;
			}

			inline Window& setWindowed(bool windowed,int pos_x,int pos_y)
			{
				this->monitor=NULL;
				glfwSetWindowMonitor(windowID,NULL,pos_x,pos_y,width,height,0);
				return *this;
			}

			inline Window& setMonitor(GLFWmonitor* monitor)
			{
				this->monitor=monitor;
				const GLFWvidmode* mode=glfwGetVideoMode(monitor);
				glfwSetWindowMonitor(windowID,monitor,0,0,mode->width,mode->height,mode->refreshRate);
				return *this;
			}

			inline Window& setWindowSize(int width_,int height_)
			{
				width=width_;
				height=height_;
				glfwSetWindowSize(windowID,width_,height_);
				return *this;
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

#ifdef GLFW_MOUSE_PASSTHROUGH
			inline void setWindowMouseEventPassthrough(bool passthrough) //设置窗口透明部分鼠标事件是否穿透，glfw3.4版本及以上可用
			{
				glfwWindowHint(GLFW_MOUSE_PASSTHROUGH,passthrough);
			}
#endif
		};
	}
}

#endif//ANDROMEDA_APP_WINDOW
