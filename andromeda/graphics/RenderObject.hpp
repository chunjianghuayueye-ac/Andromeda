#ifndef ANDROMEDA_GRAPHICS_RENDEROBJECT
#define ANDROMEDA_GRAPHICS_RENDEROBJECT

#include "../../lib/opengl/glad/glad.h"
#include "../util/ArrayList.hpp"
#include "Texture2D.hpp"
#include "VertexAttribute.hpp"

namespace andromeda {
	namespace graphics {
		class RenderObject
		{
		private:
			GLuint vao=0;
			GLuint vbo=0;
			GLuint ebo=0;
			andromeda::util::ArrayList<Texture2D> textures; //索引对应不同纹理单元，默认只有一个纹理单元0。纹理均为0级纹理，minimap靠程序自动生成
			VertexAttribute* vertex_attribs=nullptr;

		public:
			RenderObject() :
					textures(16)
			{
			}

			void loadTextures(bool generate_minimap=true,bool release_image=true); //加载所有纹理到对应的纹理单元
			void useTextures(); //使用所有纹理（在不同纹理单元中）

			inline void addTextures(Texture2D texture)
			{
				textures.add(texture);
			}

			inline void addTextures(Texture2D texture1,Texture2D texture2)
			{
				textures.add(texture1,texture2);
			}

			template<typename ...Textures>
			inline void addTextures(Texture2D texture,Textures ... textures)
			{
				textures.add(texture,textures...);
			}
		};
	}
}

#endif//ANDROMEDA_GRAPHICS_RENDEROBJECT