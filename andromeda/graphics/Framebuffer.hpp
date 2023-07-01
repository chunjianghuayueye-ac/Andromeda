#ifndef ANDROMEDA_GRAPHICS_FRAMEBUFFER
#define ANDROMEDA_GRAPHICS_FRAMEBUFFER

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

			inline void clearAll()//清除所有缓存数据
			{
				glClear(GL_DEPTH_BUFFER_BIT);
				glClear(GL_STENCIL_BUFFER_BIT);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			inline void clear()//只清除颜色缓冲
			{
				glClear(GL_COLOR_BUFFER_BIT);
			}

			void alloc(bool try_again=true); //分配内存，需要在使用前最先调用。try_again用于分配失败时是否重新分配一次
			GLuint use(); //调用后将渲染到该Framebuffer对象上，返回之前绑定的Framebuffer的id
			GLuint copy();

			void blitToScreen(); //渲染完成后调用该函数将数据刷入屏幕缓存
		};
	}
}

#endif //ANDROMEDA_GRAPHICS_FRAMEBUFFER
