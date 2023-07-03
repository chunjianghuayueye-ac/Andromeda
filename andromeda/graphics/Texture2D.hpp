#ifndef ANDROMEDA_GRAPHICS_TEXTURE2D
#define ANDROMEDA_GRAPHICS_TEXTURE2D

#include "../../lib/opengl/glad/glad.h"
#include "../image/RasterImage.hpp"

namespace andromeda {
	namespace graphics {
		class Texture2D
		{
		private:
			GLuint texture_id=0;
			andromeda::image::RasterImage image;
		public:
			__attribute__((always_inline)) operator GLuint()
			{
				return texture_id;
			}

			__attribute__((always_inline)) inline Texture2D(GLuint exist_texture_id) :
					texture_id(exist_texture_id)
			{
			}

			__attribute__((always_inline)) inline Texture2D()
			{
				glGenTextures(1,&texture_id);
			}

			Texture2D(andromeda::image::RasterImage img);
			Texture2D(const char* img_path);

			__attribute__((always_inline)) inline void setImage(andromeda::image::RasterImage img)
			{
				image=img;
			}

			__attribute__((always_inline)) inline void setImage(const char* img_path)
			{
				image=andromeda::image::RasterImage::readImage(img_path);
			}

			__attribute__((always_inline)) inline void releaseImage() //释放之后不可再load()，需要重新调用setImage()设置数据才可
			{
				image.release();
			}

			__attribute__((always_inline))  inline Texture2D& setParameter(GLint para_name,GLint para_value) //设置之前先调用use()
			{
				glTexParameteri(GL_TEXTURE_2D,para_name,para_value);
				return *this;
			}

			void load(int level=0,bool generate_minimap=true,bool release_image=true); //实际加载图片进OpenGL内存，加载后image的数据即可释放，release_image决定是否调用releaseImage();
			void load(int texture_unit,int level=0,bool generate_minimap=true,bool release_image=true);

			__attribute__((always_inline)) inline void use(void)
			{
				glBindTexture(GL_TEXTURE_2D,texture_id);
			}

			__attribute__((always_inline)) inline void use(int texture_unit)
			{
				glActiveTexture(GL_TEXTURE0+texture_unit);
				glBindTexture(GL_TEXTURE_2D,texture_id);
			}

			static inline GLint getCurrentTexture2D() //返回当前OpenGL绑定的Texture2D的id，查询代价高昂避免频繁调用
			{
				GLint current_texture2d;
				glGetIntegerv(GL_TEXTURE_BINDING_2D,&current_texture2d);
				return current_texture2d;
			}
		};
	}
}

#endif//ANDROMEDA_GRAPHICS_TEXTURE2D
