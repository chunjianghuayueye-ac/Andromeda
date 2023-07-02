#ifndef ANDROMEDA_GRAPHICS_FRAMEBUFFER
#define ANDROMEDA_GRAPHICS_FRAMEBUFFER

#include "../../lib/opengl/glad/glad.h"
#include "../image/color/ColorRGBA.hpp"

//BUFFER_SIZE宏用于得到分配内存使用的实际尺寸，在Mac OS上尺寸要乘以2
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
			GLuint frame_buffer; //默认的屏幕缓冲为0
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
			//OpenGL的查询代价高昂，应当尽量避免查询
			static inline GLuint getCurrentFramebuffer()
			{
				GLuint current_frame_buffer;
				glGetIntegerv(GL_FRAMEBUFFER_BINDING,&current_frame_buffer);
				return current_frame_buffer;
			}

			static GLuint getFramebufferTexture(GLuint dest_frame_buffer,int texture_attachment=0); //传入颜色缓冲序号，如果传入dest_frame_buffer=0（默认帧缓冲）则依照texture_attachment次序由0-3返回GL_FRONT_LEFT,GL_BACK_LEFT,GL_FRONT_RIGHT,GL_BACK_RIGHT
			static GLuint getFramebufferDepthStencil(GLuint dest_frame_buffer); //传入渲染缓冲序号

			inline GLuint getFramebufferTexture(int texture_attachment=0)
			{
				return getFramebufferTexture(frame_buffer,texture_attachment);
			}

			inline GLuint getFramebufferDepthStencil()
			{
				return getFramebufferDepthStencil(frame_buffer);
			}

			inline void clearAll() //清除所有缓存数据
			{
				glClear(GL_DEPTH_BUFFER_BIT);
				glClear(GL_STENCIL_BUFFER_BIT);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			inline void clear() //只清除颜色缓冲
			{
				glClear(GL_COLOR_BUFFER_BIT);
			}

			void alloc(bool try_again=true); //分配内存，需要在使用前最先调用。try_again用于分配失败时是否重新分配一次
			GLuint use_ret(); //调用后将渲染到该Framebuffer对象上，返回之前绑定的Framebuffer的id
			void copyColorBuffer(GLuint dest_frame_buffer,int color_buffer_attachment=0); //将本帧缓冲拷贝到另一个帧缓存上

			void renderToScreen(); //渲染完成后调用该函数将数据刷入屏幕缓存
			void blitToScreen();

			inline void use()
			{
				glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
			}
		};
	}
}

#endif //ANDROMEDA_GRAPHICS_FRAMEBUFFER
