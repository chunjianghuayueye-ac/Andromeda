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
			andromeda::util::ArrayList<Texture2D> textures; //������Ӧ��ͬ����Ԫ��Ĭ��ֻ��һ������Ԫ0�������Ϊ0������minimap�������Զ�����
			VertexAttribute* vertex_attribs=nullptr;

		public:
			RenderObject() :
					textures(16)
			{
			}

			void loadTextures(bool generate_minimap=true,bool release_image=true); //��������������Ӧ������Ԫ
			void useTextures(); //ʹ�����������ڲ�ͬ����Ԫ�У�

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