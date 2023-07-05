#ifndef ANDROMEDA_GRAPHICS_DOUBLEFRAMEBUFFER
#define ANDROMEDA_GRAPHICS_DOUBLEFRAMEBUFFER

#include "../image/ColorRGBA.hpp"
#include "Framebuffer.hpp"

namespace andromeda {
	namespace graphics {
		//初始化对象后，调用alloc()分配内存，每次渲染循环开始调用use()，渲染循环结束调用swap()即可
		class DoubleFramebuffer
		{
		private:
			Framebuffer back_frame_buffer;
		public:
			DoubleFramebuffer()=default;

			DoubleFramebuffer(int width,int height,andromeda::image::ColorRGBA clearColor={0,0,0,0});

			void alloc(bool try_again=true); //分配内存

			operator Framebuffer*()
			{
				return &back_frame_buffer;
			}

			operator Framebuffer&()
			{
				return back_frame_buffer;
			}

			inline void setClearColor(andromeda::image::ColorRGBA clearColor={0,0,0,0})
			{
				back_frame_buffer.setClearColor(clearColor);
			}

			inline void setFramebufferWidth(int new_width)
			{
				back_frame_buffer.setFramebufferWidth(new_width);
			}

			inline void setFramebufferHeight(int new_height)
			{
				back_frame_buffer.setFramebufferHeight(new_height);
			}

			inline int getFramebufferWidth()
			{
				return back_frame_buffer.getFramebufferWidth();
			}

			inline int getFramebufferHeight()
			{
				return back_frame_buffer.getFramebufferHeight();
			}

			__attribute__((always_inline)) inline void use()
			{
				back_frame_buffer.use();
			}

			__attribute__((always_inline)) inline void swap()
			{
				back_frame_buffer.blitToScreen(); //当前帧渲染到屏幕
				glFlush();
				back_frame_buffer.use();
				back_frame_buffer.clear(); //清除当前帧的内容
			}
		};
	}
}

#endif//ANDROMEDA_GRAPHICS_DOUBLEFRAMEBUFFER
