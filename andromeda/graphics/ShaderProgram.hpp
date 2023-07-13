#ifndef ANDROMEDA_GRAPHICS_SHADERPROGRAM
#define ANDROMEDA_GRAPHICS_SHADERPROGRAM

#include "../../lib/opengl/glad/glad.h"
#include "../math/Vector.hpp"
#include "../math/Matrix.hpp"

namespace andromeda {
	namespace graphics {
		extern const char* pct_pass_vertex_shader;
		extern const char* pct_default_fragment_shader;
		extern const char* pt_pass_vertex_shader;
		extern const char* pt_default_fragment_shader;
		class ShaderProgram
		{
		private:
			GLuint shader_program=0,vertex_shader=0,fragment_shader=0;
			const char* vertex_shader_source=nullptr;
			const char* fragment_shader_source=nullptr;
			static ShaderProgram* pct_default_shader_program;
			static ShaderProgram* pt_default_shader_program;

			ShaderProgram(GLuint shader_program);
			ShaderProgram(GLuint shader_program,GLuint vertex_shader,GLuint fragment_shader);
		public:
			operator GLuint()
			{
				return shader_program;
			}

			ShaderProgram(const char* vertex_shader_source=pct_pass_vertex_shader,const char* fragment_shader_source=pct_default_fragment_shader);
			//���롢����ʧ���򲻸ı�shader_program,vertex_shader,fragment_shader��ֵ
			bool setVertexShader(const char* vertex_shader_source=pct_pass_vertex_shader); //����ɫ����������ɺ��Զ����ӣ�����ֵΪ��ǰ����״̬��true��ʾ�޴���false��ʾ�д��󲻿��ã������������Ӵ���
			bool setFragmentShader(const char* fragment_shader_source=pct_default_fragment_shader);
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

			__attribute__((always_inline))  static inline ShaderProgram& getPCTDefaultShaderProgram() //��ȡĬ����ɫ����
			{
				if(!pct_default_shader_program)
					pct_default_shader_program=new ShaderProgram(pct_pass_vertex_shader,pct_default_fragment_shader);
				return *pct_default_shader_program;
			}

			__attribute__((always_inline))  static inline ShaderProgram& getPTDefaultShaderProgram() //��ȡĬ����ɫ����
			{
				if(!pt_default_shader_program)
					pt_default_shader_program=new ShaderProgram(pct_pass_vertex_shader,pct_default_fragment_shader);
				return *pt_default_shader_program;
			}

			//������ż�����ñ���ֵ��glGetUniformLocation��ѯ���۸߰�����ѭ�����ã���������ǰ�󲻸ı䵱ǰ��ɫ������
			void setInt(const char* name,int value); //���ó����еı���ֵ��OpenGL��֧�ִ���ɫ�����򷵻ر���ֵ
			void setUnsignedInt(const char* name,unsigned int value);
			void setFloat(const char* name,float value);
			void setIntArray(const char* name,int count,int* value_arr);
			void setUnsignedIntArray(const char* name,int count,unsigned int* value_arr);
			void setFloatArray(const char* name,int count,float* value_arr);
			void setMatrix4x4fArray(const char* name,int count,bool transpose,const float* value);
			void setBool(const char* name,bool value); //����setUnsignedInt����Ϊ0��1
			void setVector3f(const char* name,andromeda::math::Vector3f vec3);
			void setMatrix3x3f(const char* name,andromeda::math::Matrix3x3f& mat3,bool transpose=false);
			void setMatrix3x3f(const char* name,andromeda::math::Matrix3x3f* mat3,bool transpose=false);

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
					glUniform1iv(var_loc,count,value_arr);
				}

				inline void setArray(int count,unsigned int* value_arr)
				{
					glUniform1uiv(var_loc,count,value_arr);
				}

				inline void setArray(int count,float* value_arr)
				{
					glUniform1fv(var_loc,count,value_arr);
				}

				inline void setVector(andromeda::math::Vector3f vec3)
				{
					glUniform1fv(var_loc,3,(const GLfloat*)&vec3);
				}

				inline void setMatrix(andromeda::math::Matrix3x3f& mat3,bool transpose=false)
				{
					glUniformMatrix3fv(var_loc,1,transpose,(const GLfloat*)&mat3);
				}

				inline void setMatrix(andromeda::math::Matrix3x3f* mat3,bool transpose=false)
				{
					glUniformMatrix3fv(var_loc,1,transpose,(const GLfloat*)mat3);
				}
			};

			Variable getVariable(const char* name)
			{
				return Variable(shader_program,name);
			}
		};
	}
}

#endif //ANDROMEDA_GRAPHICS_SHADERPROGRAM
