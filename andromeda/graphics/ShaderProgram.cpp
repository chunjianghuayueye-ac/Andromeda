#include "ShaderProgram.hpp"
#include "../macros/Debug.h"

using namespace andromeda::graphics;

ShaderProgram::ShaderProgram(GLuint shader_program,GLuint vertex_shader,GLuint fragment_shader) :
		shader_program(shader_program), vertex_shader(vertex_shader), fragment_shader(fragment_shader)
{
	if((!checkShaderProgram(false))&&checkVertexShader(false)&&checkFragmentShader(false)) //shader_program无效，但所有着色器都有效则链接着色器程序
		linkProgram();
}

ShaderProgram::ShaderProgram(GLuint shader_program) :
		shader_program(shader_program)
{
}

ShaderProgram::ShaderProgram(const char* vertex_shader_source,const char* fragment_shader_source)
{
	setVertexShader(vertex_shader_source);
	setFragmentShader(fragment_shader_source);
}

bool ShaderProgram::checkVertexShader(GLuint vertex_shader,bool print_log)
{
	int success;
	glGetShaderiv(vertex_shader,GL_COMPILE_STATUS,&success);
	if(!success&&print_log)
	{
		char info[512];
		glGetShaderInfoLog(vertex_shader,512,NULL,info);
		PRINT_MESSAGE("Vertex Shader compile failed:",info,"\nID:",vertex_shader)
	}
	return (bool)success;
}

bool ShaderProgram::checkFragmentShader(GLuint fragment_shader,bool print_log)
{
	int success;
	glGetShaderiv(fragment_shader,GL_COMPILE_STATUS,&success);
	if(!success&&print_log)
	{
		char info[512];
		glGetShaderInfoLog(fragment_shader,512,NULL,info);
		PRINT_MESSAGE("Fragment Shader compile failed:",info,"\nID:",fragment_shader)
	}
	return (bool)success;
}

bool ShaderProgram::checkShaderProgram(GLuint shader_program,bool print_log)
{
	int success;
	glGetProgramiv(shader_program,GL_LINK_STATUS,&success);
	if(!success&&print_log)
	{
		char info[512];
		glGetProgramInfoLog(shader_program,512,NULL,info);
		PRINT_MESSAGE("Shader Program link failed:",info,"\nID:",shader_program)
	}
	return (bool)success;
}

bool ShaderProgram::setVertexShader(const char* vertex_shader_source)
{
	GLuint new_vertex_shader=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(new_vertex_shader,1,&vertex_shader_source,NULL);
	glCompileShader(new_vertex_shader);
	return setVertexShader(new_vertex_shader);
}

bool ShaderProgram::setFragmentShader(const char* fragment_shader_source)
{
	GLuint new_fragment_shader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(new_fragment_shader,1,&fragment_shader_source,NULL);
	glCompileShader(new_fragment_shader);
	return setFragmentShader(new_fragment_shader);
}

bool ShaderProgram::setVertexShader(GLuint vertex_shader)
{
	bool vertexShaderState=checkVertexShader(vertex_shader,false);
	if(vertexShaderState)
	{
		glDeleteShader(this->vertex_shader);
		this->vertex_shader=vertex_shader;
	}
	else
	{
		glDeleteShader(vertex_shader);
		return false;
	}
	if(checkFragmentShader(false)) //如果所有着色器均编译成功则链接
		return linkProgram();
	return vertexShaderState;
}

bool ShaderProgram::setFragmentShader(GLuint fragment_shader)
{
	bool fragmentShaderState=checkFragmentShader(false);
	if(fragmentShaderState)
	{
		glDeleteShader(this->fragment_shader);
		this->fragment_shader=fragment_shader;
	}
	else
	{
		glDeleteShader(fragment_shader);
		return false;
	}
	if(checkVertexShader(false)) //如果所有着色器均编译成功则链接
		return linkProgram();
	return fragmentShaderState;
}

bool ShaderProgram::linkProgram(bool release_shader)
{
	GLuint old_program=shader_program;
	if(checkVertexShader(false)&&checkFragmentShader(false)) //所有着色器都编译成功后才执行链接操作
	{
		shader_program=glCreateProgram();
		glAttachShader(shader_program,vertex_shader);
		glAttachShader(shader_program,fragment_shader);
		glLinkProgram(shader_program);
		bool linkState=checkShaderProgram(false);
		if(linkState)
		{
			glDeleteProgram(old_program);
			if(release_shader)
				releaseShader();
			return true;
		}
		else
		{
			glDeleteProgram(shader_program);
			shader_program=old_program;
			return false;
		}
	}
	return false;
}

GLuint ShaderProgram::use_ret()
{
	GLint last_shader_program;
	glGetIntegerv(GL_CURRENT_PROGRAM,&last_shader_program);
	glUseProgram(shader_program);
	return (GLuint)last_shader_program;
}

void ShaderProgram::releaseShader()
{
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void ShaderProgram::setInt(const char* name,int value)
{
	GLuint last_shader_program=use_ret(); //使用glUniform*更新变量值之前必须使用该着色器
	glUniform1i(glGetUniformLocation(shader_program,name),value);
	glUseProgram(last_shader_program);
}

void ShaderProgram::setUnsignedInt(const char* name,unsigned int value)
{
	GLuint last_shader_program=use_ret(); //使用glUniform*更新变量值之前必须使用该着色器
	glUniform1ui(glGetUniformLocation(shader_program,name),value);
	glUseProgram(last_shader_program);
}

void ShaderProgram::setBool(const char* name,bool value)
{
	GLuint last_shader_program=use_ret(); //使用glUniform*更新变量值之前必须使用该着色器
	glUniform1ui(glGetUniformLocation(shader_program,name),value?1:0);
	glUseProgram(last_shader_program);
}

void ShaderProgram::setFloat(const char* name,float value)
{
	GLuint last_shader_program=use_ret(); //使用glUniform*更新变量值之前必须使用该着色器
	glUniform1f(glGetUniformLocation(shader_program,name),value);
	glUseProgram(last_shader_program);
}

void ShaderProgram::setIntArray(const char* name,int count,int* value_arr)
{
	GLuint last_shader_program=use_ret(); //使用glUniform*更新变量值之前必须使用该着色器
	glUniform1iv(glGetUniformLocation(shader_program,name),count,value_arr);
	glUseProgram(last_shader_program);
}

void ShaderProgram::setUnsignedIntArray(const char* name,int count,unsigned int* value_arr)
{
	GLuint last_shader_program=use_ret(); //使用glUniform*更新变量值之前必须使用该着色器
	glUniform1uiv(glGetUniformLocation(shader_program,name),count,value_arr);
	glUseProgram(last_shader_program);
}

void ShaderProgram::setFloatArray(const char* name,int count,float* value_arr)
{
	GLuint last_shader_program=use_ret(); //使用glUniform*更新变量值之前必须使用该着色器
	glUniform1fv(glGetUniformLocation(shader_program,name),count,value_arr);
	glUseProgram(last_shader_program);
}

void ShaderProgram::setMatrix4fv(const char* name,int count,bool transpose,const float* value)
{
	GLuint last_shader_program=use_ret(); //使用glUniform*更新变量值之前必须使用该着色器
	glUniformMatrix4fv(glGetUniformLocation(shader_program,name),count,transpose,value);
	glUseProgram(last_shader_program);
}

ShaderProgram* ShaderProgram::default_shader_program=nullptr;

namespace andromeda {
	namespace graphics {
		//传递顶点着色器
		const char* pass_vertex_shader="#version 330 core\n\
										layout (location=0) in vec3 vertex_position;\n\
										layout (location=1) in vec4 vertex_color;\n\
										layout (location=2) in vec2 vertex_texture_coord;\n\
										out vec4 color;\n\
										out vec2 texture_coord;\n\
										void main()\n\
										{\n\
											gl_Position=vec4(position,1.0);\n\
											color=vertex_color;\n\
											texture_coord=vertex_texture_coord;\n\
										}";
		//默认片段着色器
		const char* default_fragment_shader="#version 330 core\n\
											in vec4 color;\n\
											in vec2 texture_coord;\n\
											out vec4 fragment_color;\n\
											uniform sampler2D texture_2d;\n\
											void main()\n\
											{\n\
												fragment_color=texture(texture_2d,texture_coord)*color;\n\
											}";
	}
}
