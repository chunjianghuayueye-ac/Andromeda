#ifndef ANDROMEDA_GRAPHICS_FRAMEBUFFER
#define ANDROMEDA_GRAPHICS_FRAMEBUFFER

#include "../../lib/opengl/glad/glad.h"
#include "../image/ColorRGBA.hpp"

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
			andromeda::image::ColorRGBA clearColor;
			GLuint frame_vbo; //������Ⱦ����Ļʹ��
			GLuint frame_ebo;

		public:
			//������Ϊ���½ǡ����Ͻǡ����½ǡ����Ͻǣ�����λ�ã�NDC���꣩��������ɫ����������
			static float full_screen_ndc_vertices_data[];
			static unsigned int full_screen_ndc_vertices_elements[4];

			inline operator GLuint()
			{
				return frame_buffer;
			}

			Framebuffer()=default; //���캯������ʵ�ʳ�ʼ���ͷ��仺��id���⽫��alloc()��ִ��
			//���캯�����ѣ�����Ĺ��캯����ͷ�ļ��ж��������������������.cpp�ļ��ж��������ʿ�ָ���쳣
			Framebuffer(int width,int height,andromeda::image::ColorRGBA clearColor={0,0,0,0}) :
					width(width), height(height)
			{
				setClearColor(clearColor);
			}

			inline void setClearColor(andromeda::image::ColorRGBA clearColor={0,0,0,0})
			{
				this->clearColor=clearColor;
				glClearColor(clearColor.r,clearColor.g,clearColor.b,clearColor.a);
			}

			inline andromeda::image::ColorRGBA getClearColor()
			{
				return clearColor;
			}

			inline void setFramebufferWidth(int new_width)
			{
				width=new_width;
			}

			inline void setFramebufferHeight(int new_height)
			{
				height=new_height;
			}

			inline int getFramebufferWidth()
			{
				return width;
			}

			inline int getFramebufferHeight()
			{
				return height;
			}
			//OpenGL�Ĳ�ѯ���۸߰���Ӧ�����������ѯ
			static GLuint getCurrentFramebuffer()
			{
				GLint current_frame_buffer;
				glGetIntegerv(GL_FRAMEBUFFER_BINDING,&current_frame_buffer);
				return (GLuint)current_frame_buffer;
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

			__attribute__((always_inline)) inline void clearAll() //������л�������
			{
				glClear(GL_DEPTH_BUFFER_BIT);
				glClear(GL_STENCIL_BUFFER_BIT);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			__attribute__((always_inline)) inline void clear() //ֻ�����ɫ����
			{
				glClear(GL_COLOR_BUFFER_BIT);
			}

			void alloc(bool try_again=true); //�����ڴ棬��Ҫ��ʹ��ǰ���ȵ��á�try_again���ڷ���ʧ��ʱ�Ƿ����·���һ��
			GLuint use_ret(); //���ú���Ⱦ����Framebuffer�����ϣ�����֮ǰ�󶨵�Framebuffer��id
			void copyColorBuffer(GLuint dest_frame_buffer,int color_buffer_attachment=0); //����֡���忽������һ��֡�����ϡ�dest_frame_buffer=0�򿽱�����Ļ���壬��ʱcolor_buffer_attachment��������

			//��Ⱦ��ɺ���øú�����������Ⱦ����Ļ���沿������(����NDC����)����Ⱦ��frame_buffer�󶨵�0����Ļ���壩
			void renderToScreen(float* vertex_arr); //vertex_arr��Ҫָ���ĸ��㣨������Ϊ���½ǡ����Ͻǡ����½ǡ����Ͻǣ�����λ�á���ɫ���������꣩������Ⱦ����Ļ���壬ͨ������Ҫ�����������
			__attribute__((always_inline)) inline void blitToScreen() //��ȫ�����Ļ���棬�ڲ�ʹ��renderToScreen()ʵ��
			{
				renderToScreen(full_screen_ndc_vertices_data);
			}

			__attribute__((always_inline)) inline void use()
			{
				glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
			}
		};
	}
}

#endif //ANDROMEDA_GRAPHICS_FRAMEBUFFER
