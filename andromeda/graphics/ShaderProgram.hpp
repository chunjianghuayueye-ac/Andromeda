#ifndef ANDROMEDA_GRAPHICS_SHADERPROGRAM
#define ANDROMEDA_GRAPHICS_SHADERPROGRAM

#include "../../lib/opengl/glad/glad.h"

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
			//���롢����ʧ���򲻸ı�shader_program,vertex_shader,fragment_shader��ֵ
			bool setVertexShader(const char* vertex_shader_source=pass_vertex_shader); //����ɫ����������ɺ��Զ����ӣ�����ֵΪ��ǰ����״̬��true��ʾ�޴���false��ʾ�д��󲻿��ã������������Ӵ���
			bool setFragmentShader(const char* fragment_shader_source=default_fragment_shader);
			bool setVertexShader(GLuint vertex_shader); //�����Ѿ�����õ���ɫ�������checkVertexShader()����false˵����ɫ��ʵ�����д���ú�������false����������ʧ��Ҳ����false
			bool setFragmentShader(GLuint fragment_shader);
			bool linkProgram(bool release_shader=true); //ͨ������Ҫ���ã������Զ����ӣ��ɹ��򷵻�true��release_shader����������ӳɹ��Ƿ��ͷ��Ѿ��������ɫ����Դ��Ĭ���ͷ���Դ
			GLuint use_ret(); //ʹ�ø���ɫ����������һ��ʹ�õ���ɫ��
			void releaseShader(); //�ͷ��Ѿ��������ɫ����Դ

			__attribute__((always_inline)) inline void use() //ʹ�ø���ɫ��
			{
				glUseProgram(shader_program);
			}

			static bool checkVertexShader(GLuint vertex_shader,bool print_log=true); //�ɹ��򷵻�true��print_log������������Ƿ��ӡ������Ϣ�������
			static bool checkFragmentShader(GLuint fragment_shader,bool print_log=true); //�ɹ��򷵻�true
			static bool checkShaderProgram(GLuint shader_program,bool print_log=true); //�ɹ��򷵻�true

			__attribute__((always_inline)) inline bool checkVertexShader(bool print_log=true) //�ɹ��򷵻�true��print_log������������Ƿ��ӡ������Ϣ�������
			{
				return checkVertexShader(vertex_shader,print_log);
			}

			__attribute__((always_inline)) inline bool checkFragmentShader(bool print_log=true) //�ɹ��򷵻�true
			{
				return checkFragmentShader(fragment_shader,print_log);
			}

			__attribute__((always_inline)) inline bool checkShaderProgram(bool print_log=true) //�ɹ��򷵻�true
			{
				return checkShaderProgram(shader_program,print_log);
			}

			__attribute__((always_inline))  static inline ShaderProgram& getDefaultShaderProgram() //��ȡĬ����ɫ����
			{
				return default_shader_program;
			}

			//������ż�����ñ���ֵ��glGetUniformLocation��ѯ���۸߰�����ѭ�����ã���������ǰ�󲻸ı䵱ǰ��ɫ������
			void setInt(const char* name,int value); //���ó����еı���ֵ��OpenGL��֧�ִ���ɫ�����򷵻ر���ֵ
			void setUnsignedInt(const char* name,unsigned int value);
			void setFloat(const char* name,float value);
			void setIntArray(const char* name,int count,int* value_arr);
			void setUnsignedIntArray(const char* name,int count,unsigned int* value_arr);
			void setFloatArray(const char* name,int count,float* value_arr);
			void setMatrix4fv(const char* name,int count,bool transpose,const float* value);
			void setBool(const char* name,bool value); //����setUnsignedInt����Ϊ0��1

			//������Ƶ�����ñ���ֵ��ʹ��ǰʹ��use()�趨��ɫ����ſ��Ե���set()��setArray()
			class Variable
			{
			private:
				GLuint shader_program;
				GLint var_loc;
			public:
				operator GLint()
				{
					return var_loc;
				}

				inline Variable(GLuint shader_program,const char* name) :
						shader_program(shader_program)
				{
					var_loc=glGetUniformLocation(shader_program,name);
				}

				inline void use()
				{
					glUseProgram(shader_program);
				}

				inline void set(int value)
				{
					glUniform1i(var_loc,value);
				}

				inline void set(unsigned int value)
				{
					glUniform1ui(var_loc,value);
				}

				inline void set(float value)
				{
					glUniform1f(var_loc,value);
				}

				inline void setArray(int count,int* value_arr)
				{
					glUniform1iv(var_loc,value_arr);
				}

				inline void setArray(int count,unsigned int* value_arr)
				{
					glUniform1uiv(var_loc,value_arr);
				}

				inline void setArray(int count,float* value_arr)
				{
					glUniform1fv(var_loc,value_arr);
				}
			};

			Variable getVariable(const char* name)
			{
				return Variable(shader_program,name);
			}
		};

		extern ShaderProgram default_shader_program;
	}
}

#endif //ANDROMEDA_GRAPHICS_SHADERPROGRAM
