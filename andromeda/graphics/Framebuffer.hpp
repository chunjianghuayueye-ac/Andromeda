#ifndef ANDROMEDA_GRAPHICS_FRAMEBUFFER
#define ANDROMEDA_GRAPHICS_FRAMEBUFFER

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../image/color/ColorRGBA.hpp"

//BUFFER_SIZE�����ڵõ������ڴ�ʹ�õ�ʵ�ʳߴ磬��Mac OS�ϳߴ�Ҫ����2
#ifdef __APPLE__
#define BUFFER_SIZE(size) (size*2)
#else
#define BUFFER_SIZE(size) (size)
#endif

namespace andromeda {
	namespace graphics {
		class Framebuffer
		{
		private:
			GLuint frame_buffer; //Ĭ�ϵ���Ļ����Ϊ0
			GLuint color_buffer;
			GLuint depth_stencil_buffer;
			int width,height;
			bool allocated=false;
			andromeda::image::color::ColorRGBA clearColor;

		public:
			inline operator GLuint()
			{
				return frame_buffer;
			}

			Framebuffer(int width,int height,andromeda::image::color::ColorRGBA clearColor={0,0,0,0});

			inline void setClearColor(andromeda::image::color::ColorRGBA clearColor={0,0,0,0})
			{
				this->clearColor=clearColor;
				glClearColor(clearColor.r,clearColor.g,clearColor.b,clearColor.a);
			}

			inline andromeda::image::color::ColorRGBA getClearColor()
			{
				return clearColor;
			}

			inline void clearAll()//������л�������
			{
				glClear(GL_DEPTH_BUFFER_BIT);
				glClear(GL_STENCIL_BUFFER_BIT);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			inline void clear()//ֻ�����ɫ����
			{
				glClear(GL_COLOR_BUFFER_BIT);
			}

			void alloc(bool try_again=true); //�����ڴ棬��Ҫ��ʹ��ǰ���ȵ��á�try_again���ڷ���ʧ��ʱ�Ƿ����·���һ��
			GLuint use(); //���ú���Ⱦ����Framebuffer�����ϣ�����֮ǰ�󶨵�Framebuffer��id
			GLuint copy();

			void blitToScreen(); //��Ⱦ��ɺ���øú���������ˢ����Ļ����
		};
	}
}

#endif //ANDROMEDA_GRAPHICS_FRAMEBUFFER
