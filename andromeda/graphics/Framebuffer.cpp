#include "Framebuffer.hpp"
#include "../macros/Debug.h"
#include "ShaderProgram.hpp"
#include "VertexAttribute.hpp"

using namespace andromeda::graphics;
using namespace andromeda::image;

Framebuffer::Framebuffer(int width,int height,ColorRGBA clearColor) :
		width(width), height(height), clearColor(clearColor)
{
}

void Framebuffer::alloc(bool try_again)
{
	bool tried=false; //是否已尝试重新分配
	ALLOC:if((!tried)&&allocated)
		clearAll();
	else
	{
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
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glBindTexture(GL_TEXTURE_2D,0);
		glBindRenderbuffer(GL_RENDERBUFFER,0);
		glGenBuffers(1,&frame_vertex_buffer);
		glGenBuffers(1,&frame_element_buffer);
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

GLuint Framebuffer::getFramebufferTexture(GLuint dest_frame_buffer,int texture_attachment) //传入颜色缓冲序号
{
	GLint dest_texture;
	glBindFramebuffer(GL_FRAMEBUFFER,dest_frame_buffer);
	if(dest_frame_buffer) //用户创建缓冲
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+texture_attachment,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&dest_texture);
	else
		//默认缓冲
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,GL_FRONT_LEFT+texture_attachment,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&dest_texture);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	return (GLuint)dest_texture;
}

GLuint Framebuffer::getFramebufferDepthStencil(GLuint dest_frame_buffer) //传入颜色缓冲序号
{
	GLint dest_depth_stencil;
	glBindFramebuffer(GL_FRAMEBUFFER,dest_frame_buffer);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&dest_depth_stencil);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	return (GLuint)dest_depth_stencil;
}

void Framebuffer::copyColorBuffer(GLuint dest_frame_buffer,int color_buffer_attachment)
{
	if(dest_frame_buffer)
	{
		GLuint dest_texture=getFramebufferTexture(dest_frame_buffer,color_buffer_attachment);
		glBindFramebuffer(GL_FRAMEBUFFER,dest_frame_buffer);
		glBindTexture(GL_TEXTURE_2D,dest_texture);
		glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,width,height);
		glBindTexture(GL_TEXTURE_2D,0);
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
	else
		blitToScreen();
}

GLuint Framebuffer::use_ret()
{
	GLuint last_frame_buffer=getCurrentFramebuffer();
	glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
	return last_frame_buffer;
}

void Framebuffer::renderToScreen(float* vertex_arr)
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	VertexAttribute& vertex_attrib=VertexAttribute::getDefaultVertexAttribute();
	vertex_attrib.use();
	ShaderProgram::getDefaultShaderProgram().use();
	//绘制到屏幕
	glBindBuffer(GL_ARRAY_BUFFER,frame_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER,4*vertex_attrib.getVertexSize(),vertex_arr,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,frame_element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(full_screen_ndc_vertices_elements),full_screen_ndc_vertices_elements,GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_INT,0);
}

float Framebuffer::full_screen_ndc_vertices_data[]={-1.0f,-1.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,-1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,1.0f,1.0f,-1.0f,0.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};
unsigned int Framebuffer::full_screen_ndc_vertices_elements[4]={0,1,2,3};
