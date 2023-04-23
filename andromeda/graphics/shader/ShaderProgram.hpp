#ifndef ENGINE_GRAPHICS_SHADER_SHADERPROGRAM
#define ENGINE_GRAPHICS_SHADER_SHADERPROGRAM

#include <GL/gl.h>

namespace engine{
	namespace graphics{
		class ShaderProgram
		{
			GLuint shader_program,vertex_shader,fragment_shader;
			const char* vertex_shader_source,fragment_shader_source;

		public:

		};
	}
}

#endif //ENGINE_GRAPHICS_SHADER_SHADERPROGRAM
