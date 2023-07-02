#include "Framebuffer.hpp"
#include "../macros/Debug.h"
#include "ShaderProgram.hpp"

using namespace andromeda::graphics;
using namespace andromeda::image::color;

Framebuffer::Framebuffer(int width,int height,ColorRGBA clearColor) :
		width(width), height(height), clearColor(clearColor)
{
}

void Framebuffer::alloc(bool try_again)
{
	bool tried=false; //�Ƿ��ѳ������·���
	ALLOC:if((!tried)&&allocated)
		clearAll();
	else
	{
		glGenFramebuffers(1,&frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
		glGenTextures(1,&color_buffer);
		glBindTexture(GL_TEXTURE_2D,color_buffer);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,BUFFER_SIZE(width),BUFFER_SIZE(height),0,GL_RGBA,GL_UNSIGNED_BYTE,NULL); //Ϊ��ɫ��������ڴ�
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //������ɫ���������ʽ
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,color_buffer,0); //����ɫ����󶨵�frame_buffer
		glGenRenderbuffers(1,&depth_stencil_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER,depth_stencil_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,BUFFER_SIZE(width),BUFFER_SIZE(height)); //Ϊ���ģ�建������ڴ�
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,depth_stencil_buffer); //�����ģ�建��󶨵�frame_buffer
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glBindTexture(GL_TEXTURE_2D,0);
		glBindRenderbuffer(GL_RENDERBUFFER,0);
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

GLuint Framebuffer::getFramebufferTexture(GLuint dest_frame_buffer,int texture_attachment) //������ɫ�������
{
	GLuint dest_texture;
	glBindFramebuffer(GL_FRAMEBUFFER,dest_frame_buffer);
	if(dest_frame_buffer) //�û���������
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+texture_attachment,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&dest_texture);
	else
		//Ĭ�ϻ���
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,GL_FRONT_LEFT+texture_attachment,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&dest_texture);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	return dest_texture;
}

GLuint Framebuffer::getFramebufferDepthStencil(GLuint dest_frame_buffer) //������ɫ�������
{
	GLuint dest_depth_stencil;
	glBindFramebuffer(GL_FRAMEBUFFER,dest_frame_buffer);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&dest_depth_stencil);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	return dest_depth_stencil;
}

void Framebuffer::copyColorBuffer(GLuint dest_frame_buffer,int color_buffer_attachment)
{
	GLuint dest_texture=getFramebufferTexture(dest_frame_buffer,color_buffer_attachment);
	glBindFramebuffer(GL_FRAMEBUFFER,dest_frame_buffer);
	glBindTexture(GL_TEXTURE_2D,dest_texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,width,height);
	glBindTexture(GL_TEXTURE_2D,0);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

GLuint Framebuffer::use_ret()
{
	GLuint last_frame_buffer=getCurrentFramebuffer();
	glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
	return last_frame_buffer;
}

void Framebuffer::blitToScreen()
{

}

void Framebuffer::renderToScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
	GLuint last_shader_program=ShaderProgram::getDefaultShaderProgram().use();
	//���Ƶ���Ļ

	glBindFramebuffer(GL_FRAMEBUFFER,0);
}
