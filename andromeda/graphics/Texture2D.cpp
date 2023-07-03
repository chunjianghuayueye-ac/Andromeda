#include "Texture2D.hpp"

using namespace andromeda::graphics;
using namespace andromeda::image;

Texture2D::Texture2D(RasterImage img) :
		Texture2D()
{
	image=img;
}

Texture2D::Texture2D(const char* img_path) :
		Texture2D()
{
	image=RasterImage::readImage(img_path);
}

void Texture2D::load(int level,bool generate_minimap,bool release_image) //ʵ�ʼ���ͼƬ��OpenGL�ڴ棬���غ�image�����ݼ����ͷţ���Ҫ�ֶ�����releaseImage();
{
	use();
	if(image.getPixelDataPtr())
	{
		glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,image.getWidth(),image.getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,image);
		if(generate_minimap)
			glGenerateMipmap(GL_TEXTURE_2D);
		if(release_image)
			releaseImage();
	}
}

void Texture2D::load(int texture_unit,int level,bool generate_minimap,bool release_image)
{
	use(texture_unit);
	if(image.getPixelDataPtr())
	{
		glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,image.getWidth(),image.getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,image);
		if(generate_minimap)
			glGenerateMipmap(GL_TEXTURE_2D);
		if(release_image)
			releaseImage();
	}
}
