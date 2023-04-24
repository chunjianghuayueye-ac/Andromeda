#ifndef ANDROMEDA_GRAPHICS_SHADER_SHADERPROGRAM
#define ANDROMEDA_GRAPHICS_SHADER_SHADERPROGRAM

#include <GL/gl.h>

namespace andromeda{
	namespace graphics{
		class ShaderProgram
		{
			GLuint shader_program,vertex_shader,fragment_shader;
			const char* vertex_shader_source,fragment_shader_source;

		public:

		};
	}
}

#endif //ANDROMEDA_GRAPHICS_SHADER_SHADERPROGRAM
