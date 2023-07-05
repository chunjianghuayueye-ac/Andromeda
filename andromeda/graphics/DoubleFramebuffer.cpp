#include "DoubleFramebuffer.hpp"

using namespace andromeda::graphics;
using namespace andromeda::image;

DoubleFramebuffer::DoubleFramebuffer(int width,int height,ColorRGBA clearColor) :
		back_frame_buffer(width,height,clearColor)
{
}

void DoubleFramebuffer::alloc(bool try_again)
{
	back_frame_buffer.alloc(try_again);
}

