#ifndef ANDROMEDA_GRAPHICS_RENDEROBJECT
#define ANDROMEDA_GRAPHICS_RENDEROBJECT

#include "../../lib/opengl/glad/glad.h"
#include "../util/ArrayList.hpp"
#include "Texture2D.hpp"
#include "VertexAttribute.hpp"
#include "ShaderProgram.hpp"
#include "../math/Vector.hpp"
#include "../math/Matrix.hpp"

namespace andromeda {
	namespace graphics {
		class RenderInstance
		{
		public:
			andromeda::math::Vector3f position; //ԭ��ƽ����
			andromeda::math::Matrix3x3f* linear_transform; //��RenderObjectӦ�õ����Ա任
		};
		//ֻ������Ⱦ���屾�����Ϣ���������ݡ���ɫ��������ȣ���������λ�á��任�ȣ���Щ����ʵ������Ϣ����ͬʵ�����Թ���ͬһ��RenderObject��
		class RenderObject
		{
		private:
			GLuint vao=0;
			GLuint vbo=0;
			GLuint ebo=0;
			andromeda::util::ArrayList<Texture2D> textures; //������Ӧ��ͬ����Ԫ��Ĭ��ֻ��һ������Ԫ0�������Ϊ0������minimap�������Զ�����

		public:
			VertexAttribute* vertex_attribs=nullptr;
			ShaderProgram* shader=nullptr;
			size_t vertex_count=0; //�������
			float* vertex_data=nullptr; //�������ݵ�ָ��
			size_t element_count=0; //��������
			unsigned int* element_data=nullptr; //�������ݵ�ָ�룬��ѡ
			GLuint draw_strategy=GL_TRIANGLE_STRIP; //���Ʒ�ʽ

			RenderObject() :
					textures(16)
			{
			}

			inline operator GLuint()
			{
				return vao;
			}

			inline void use()
			{
				glBindVertexArray(vao);
			}

			__attribute__((always_inline)) inline void draw(RenderInstance* instance) //loadData()��ſ��Ե��ã����򱨴�
			{
				shader->use();
				shader->setVector3f("position",instance->position);
				shader->setMatrix3x3f("linear_transform",*(instance->linear_transform));
				use();
				if(ebo)
					glDrawElements(draw_strategy,element_count,GL_UNSIGNED_INT,0);
				else
					glDrawArrays(draw_strategy,0,vertex_count);
			}

			void loadTextures(bool generate_minimap=true,bool release_image=true); //��������������Ӧ������Ԫ
			void useTextures(); //ʹ�����������ڲ�ͬ����Ԫ�У�

			void loadData(); //���ض��������Լ��������ݣ�����еĻ�����OpenGL�������ú�vertex_attribs��Ӧ�Ķ������Ը�ʽ

			template<typename ...Textures>
			inline void addTextures(Textures ... more_textures) //����ȫ������Texture2D���͵Ķ���
			{
				textures.add(more_textures...);
			}
		};
	}
}

#endif//ANDROMEDA_GRAPHICS_RENDEROBJECT