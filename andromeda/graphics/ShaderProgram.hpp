#ifndef ANDROMEDA_GRAPHICS_SHADERPROGRAM
#define ANDROMEDA_GRAPHICS_SHADERPROGRAM

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace andromeda {
	namespace graphics {
		extern const char* pass_vertex_shader;
		extern const char* default_fragment_shader;

		class ShaderProgram
		{
		private:
			GLuint shader_program=0,vertex_shader=0,fragment_shader=0;
			const char* vertex_shader_source=nullptr;
			const char* fragment_shader_source=nullptr;

			ShaderProgram(GLuint shader_program);
			ShaderProgram(GLuint shader_program,GLuint vertex_shader,GLuint fragment_shader);
		public:
			operator GLuint()
			{
				return shader_program;
			}

			ShaderProgram(const char* vertex_shader_source=pass_vertex_shader,const char* fragment_shader_source=default_fragment_shader);
			//编译、链接失败则不改变shader_program,vertex_shader,fragment_shader的值
			bool setVertexShader(const char* vertex_shader_source=pass_vertex_shader); //当着色器都编译完成后将自动链接，返回值为当前程序状态，true表示无错误，false表示有错误不可用（编译错误或链接错误）
			bool setFragmentShader(const char* fragment_shader_source=default_fragment_shader);
			bool setVertexShader(GLuint vertex_shader); //设置已经编译好的着色器，如果checkVertexShader()返回false说明着色器实际上有错，则该函数返回false。程序链接失败也返回false
			bool setFragmentShader(GLuint fragment_shader);
			bool linkProgram(bool release_shader=true); //通常不需要调用，可以自动链接，成功则返回true。release_shader决定如果链接成功是否释放已经编译的着色器资源，默认释放资源
			GLuint use_ret(); //使用该着色器并返回上一个使用的着色器
			void releaseShader(); //释放已经编译的着色器资源

			__attribute__((always_inline)) inline void use() //使用该着色器
			{
				glUseProgram(shader_program);
			}

			static bool checkVertexShader(GLuint vertex_shader,bool print_log=true); //成功则返回true。print_log决定如果出错是否打印错误信息方便调试
			static bool checkFragmentShader(GLuint fragment_shader,bool print_log=true); //成功则返回true
			static bool checkShaderProgram(GLuint shader_program,bool print_log=true); //成功则返回true

			__attribute__((always_inline)) inline bool checkVertexShader(bool print_log=true) //成功则返回true。print_log决定如果出错是否打印错误信息方便调试
			{
				return checkVertexShader(vertex_shader,print_log);
			}

			__attribute__((always_inline)) inline bool checkFragmentShader(bool print_log=true) //成功则返回true
			{
				return checkFragmentShader(fragment_shader,print_log);
			}

			__attribute__((always_inline)) inline bool checkShaderProgram(bool print_log=true) //成功则返回true
			{
				return checkShaderProgram(shader_program,print_log);
			}

			__attribute__((always_inline))     inline static ShaderProgram& getDefaultShaderProgram() //获取默认着色程序
			{
				return default_shader_program;
			}

			void setInt(const char* name,int value); //设置程序中的变量值，OpenGL不支持从着色器程序返回变量值
			void setUnsignedInt(const char* name,unsigned int value);
			void setFloat(const char* name,float value);
			void setIntArray(const char* name,int count,int* value_arr);
			void setUnsignedIntArray(const char* name,int count,unsigned int* value_arr);
			void setFloatArray(const char* name,int count,float* value_arr);
			void setMatrix4fv(const char* name,int count,bool transpose,const float* value);
			void setBool(const char* name,bool value);
		};

		extern ShaderProgram default_shader_program;
	}
}

#endif //ANDROMEDA_GRAPHICS_SHADERPROGRAM
