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
		};
	}
}

#endif//ANDROMEDA_APP_WINDOW
