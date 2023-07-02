#ifndef ANDROMEDA_GRAPHICS_TEXTURE2D
#define ANDROMEDA_GRAPHICS_TEXTURE2D

#include "../../lib/opengl/glad/glad.h"

namespace andromeda {
	namespace graphics {
		class Texture2D
		{
		public:
			GLuint texture_id;

			static inline GLuint getCurrentTexture2D() //返回当前OpenGL绑定的Texture2D的id
			{
				GLuint current_texture2d;
				glGetIntegerv(GL_TEXTURE_BINDING_2D,&current_texture2d);
				return current_texture2d;
			}
		};
	}
}

#endif//ANDROMEDA_GRAPHICS_TEXTURE2D
