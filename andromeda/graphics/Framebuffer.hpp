#ifndef GEFW_GRAPHICS_FRAMEBUFFER
#define GEFW_GRAPHICS_FRAMEBUFFER

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <malloc.h>

namespace gefw{
	namespace graphics{
		class Framebuffer{
			GLuint buffer_id;

			Framebuffer(int width,int height);
		};
	}
}

#endif //GEFW_GRAPHICS_FRAMEBUFFER
