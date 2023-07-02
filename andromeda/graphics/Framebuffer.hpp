#ifndef ANDROMEDA_GRAPHICS_FRAMEBUFFER
#define ANDROMEDA_GRAPHICS_FRAMEBUFFER

#include "../../lib/opengl/glad/glad.h"
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
			//OpenGL�Ĳ�ѯ���۸߰���Ӧ�����������ѯ
			static inline GLuint getCurrentFramebuffer()
			{
				GLuint current_frame_buffer;
				glGetIntegerv(GL_FRAMEBUFFER_BINDING,&current_frame_buffer);
				return current_frame_buffer;
			}

			static GLuint getFramebufferTexture(GLuint dest_frame_buffer,int texture_attachment=0); //������ɫ������ţ��������dest_frame_buffer=0��Ĭ��֡���壩������texture_attachment������0-3����GL_FRONT_LEFT,GL_BACK_LEFT,GL_FRONT_RIGHT,GL_BACK_RIGHT
			static GLuint getFramebufferDepthStencil(GLuint dest_frame_buffer); //������Ⱦ�������

			inline GLuint getFramebufferTexture(int texture_attachment=0)
			{
				return getFramebufferTexture(frame_buffer,texture_attachment);
			}

			inline GLuint getFramebufferDepthStencil()
			{
				return getFramebufferDepthStencil(frame_buffer);
			}

			inline void clearAll() //������л�������
			{
				glClear(GL_DEPTH_BUFFER_BIT);
				glClear(GL_STENCIL_BUFFER_BIT);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			inline void clear() //ֻ�����ɫ����
			{
				glClear(GL_COLOR_BUFFER_BIT);
			}

			void alloc(bool try_again=true); //�����ڴ棬��Ҫ��ʹ��ǰ���ȵ��á�try_again���ڷ���ʧ��ʱ�Ƿ����·���һ��
			GLuint use_ret(); //���ú���Ⱦ����Framebuffer�����ϣ�����֮ǰ�󶨵�Framebuffer��id
			void copyColorBuffer(GLuint dest_frame_buffer,int color_buffer_attachment=0); //����֡���忽������һ��֡������

			void renderToScreen(); //��Ⱦ��ɺ���øú���������ˢ����Ļ����
			void blitToScreen();

			inline void use()
			{
				glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
			}
		};
	}
}

#endif //ANDROMEDA_GRAPHICS_FRAMEBUFFER
