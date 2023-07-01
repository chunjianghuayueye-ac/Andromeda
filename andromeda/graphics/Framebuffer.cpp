#include "Framebuffer.hpp"
#include "../macros/Debug.h"
#include "ShaderProgram.hpp"

using namespace andromeda::graphics;
using namespace andromeda::image::color;

Framebuffer::Framebuffer(int width,int height,ColorRGBA clearColor) :
		width(width), height(height),clearColor(clearColor)
{
}

void Framebuffer::alloc(bool try_again)
{
	bool tried=false; //是否已尝试重新分配
	ALLOC:if((!tried)&&allocated)
		clearAll();
	else
	{
		GLuint last_frame_buffer,last_texture,last_render_buffer; //获取之前绑定的buffer id
		glGetIntegerv(GL_FRAMEBUFFER_BINDING,&last_frame_buffer);
		glGetIntegerv(GL_TEXTURE_BINDING_BUFFER,&last_texture);
		glGetIntegerv(GL_RENDERBUFFER_BINDING,&last_render_buffer);
		glGenFramebuffers(1,&frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
		glGenTextures(1,&color_buffer);
		glBindTexture(GL_TEXTURE_2D,color_buffer);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,BUFFER_SIZE(width),BUFFER_SIZE(height),0,GL_RGBA,GL_UNSIGNED_BYTE,NULL); //为颜色缓冲分配内存
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //设置颜色缓冲采样格式
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,color_buffer,0); //将颜色缓冲绑定到frame_buffer
		glGenRenderbuffers(1,&depth_stencil_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER,depth_stencil_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,BUFFER_SIZE(width),BUFFER_SIZE(height)); //为深度模板缓冲分配内存
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,depth_stencil_buffer); //将深度模板缓冲绑定到frame_buffer
		glBindFramebuffer(GL_FRAMEBUFFER,last_frame_buffer);
		glBindTexture(GL_TEXTURE_2D,last_texture);
		glBindRenderbuffer(GL_RENDERBUFFER,last_render_buffer);
	}
	allocated=true;
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
	{
		if((!try_again)||tried)
			PRINT_MESSAGE("Framebuffer initialize failed.")
		else
		{
			tried=true;
			goto ALLOC;
		}
	}
}

GLuint Framebuffer::use()
{
	GLuint last_frame_buffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING,&last_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
	return last_frame_buffer;
}

void Framebuffer::blitToScreen()
{
	GLuint last_frame_buffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING,&last_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
	GLuint last_shader_program=ShaderProgram::getDefaultShaderProgram().use_ret();
	//绘制到屏幕

	glBindFramebuffer(GL_FRAMEBUFFER,last_frame_buffer);
	glUseProgram(last_shader_program);
}
