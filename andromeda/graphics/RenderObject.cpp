#include "RenderObject.hpp"

using namespace andromeda::graphics;

void RenderObject::loadTextures(bool generate_minimap,bool release_image)
{
	for(int i=0;i<textures.getLength();++i)
		textures[i].load(i,0,generate_minimap,release_image);
}

void RenderObject::useTextures()
{
	for(int i=0;i<textures.getLength();++i)
		textures[i].use(i);
}

void RenderObject::loadData()
{
	if((!vertex_data)||(!vertex_count)) //如果没有顶点数据则直接返回
		return;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,vertex_count*vertex_attribs->getVertexSize(),vertex_data,GL_STATIC_DRAW);
	if(element_data&&element_count)
	{
		glGenBuffers(1,&ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*element_count,element_data,GL_STATIC_DRAW);
	}
	vertex_attribs->load(&vao);
}
