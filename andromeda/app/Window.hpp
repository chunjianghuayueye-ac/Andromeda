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
			Window()=default; //�ù��캯������ʵ�ʳ�ʼ�����ڽ������ڴ棬��Ҫ���µ����������캯������������
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

			inline bool setIsAlwaysOnTop(bool floating) //���ô����Ƿ������ö�
			{
				glfwSetWindowAttrib(windowID,GLFW_FLOATING,floating);
				return glfwGetWindowAttrib(windowID,GLFW_FLOATING); //���óɹ��򷵻�true
			}

			inline bool setWindowFramebufferTransparent(bool transparent=false) //���ô�����Ⱦ�ı����Ƿ�͸����ע�⼴ʹ����͸�����¼����ɲ��ᴩ͸����͸�����֣����ǻᱻ���ڲ���
			{
				glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,transparent);
				return glfwGetWindowAttrib(windowID,GLFW_TRANSPARENT_FRAMEBUFFER); //���óɹ��򷵻�true
			}

			inline float setWindowOpacity(float opacity=1) //���������͸�������ã�λ��0-1��
			{
				glfwSetWindowOpacity(windowID,opacity);
				return glfwGetWindowOpacity(windowID); //���ص�ǰ�Ĵ���͸���ȡ�������óɹ���Ӧ�÷���opacity
			}

#ifdef GLFW_MOUSE_PASSTHROUGH
			inline void setWindowMouseEventPassthrough(bool passthrough) //���ô���͸����������¼��Ƿ�͸��glfw3.4�汾�����Ͽ���
			{
				glfwWindowHint(GLFW_MOUSE_PASSTHROUGH,passthrough);
			}
#endif
		};
	}
}

#endif//ANDROMEDA_APP_WINDOW
