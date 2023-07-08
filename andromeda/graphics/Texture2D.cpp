#include "Texture2D.hpp"

using namespace andromeda::graphics;

void Texture2D::load(int level,bool generate_minimap,bool release_image) //实际加载图片进OpenGL内存，加载后image的数据即可释放，需要手动调用releaseImage();
{
	if(!texture_id)//texture_id=0才加载
		return;
	if(image.getPixelDataPtr())
	{
		glGenTextures(1,&texture_id);
		use();
		glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,image.getWidth(),image.getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,image);
		if(generate_minimap)
			glGenerateMipmap(GL_TEXTURE_2D);
		if(release_image)
			releaseImage();
	}
}

void Texture2D::load(int texture_unit,int level,bool generate_minimap,bool release_image)
{
	if(!texture_id)
		return;
	if(image.getPixelDataPtr())
	{
		glGenTextures(1,&texture_id);
		use(texture_unit);
		glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,image.getWidth(),image.getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,image);
		if(generate_minimap)
			glGenerateMipmap(GL_TEXTURE_2D);
		if(release_image)
			releaseImage();
	}
}
