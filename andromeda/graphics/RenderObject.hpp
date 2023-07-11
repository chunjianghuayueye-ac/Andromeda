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
			andromeda::math::Vector3f position; //原点平移量
			andromeda::math::Matrix3x3f* linear_transform; //对RenderObject应用的线性变换
		};
		//只包含渲染物体本身的信息（顶点数据、着色器、纹理等），不包括位置、变换等（这些属于实例的信息，不同实例可以共享同一个RenderObject）
		class RenderObject
		{
		private:
			GLuint vao=0;
			GLuint vbo=0;
			GLuint ebo=0;
			andromeda::util::ArrayList<Texture2D> textures; //索引对应不同纹理单元，默认只有一个纹理单元0。纹理均为0级纹理，minimap靠程序自动生成

		public:
			VertexAttribute* vertex_attribs=nullptr;
			ShaderProgram* shader=nullptr;
			size_t vertex_count=0; //顶点个数
			float* vertex_data=nullptr; //顶点数据的指针
			size_t element_count=0; //索引个数
			unsigned int* element_data=nullptr; //索引数据的指针，可选
			GLuint draw_strategy=GL_TRIANGLE_STRIP; //绘制方式

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

			__attribute__((always_inline)) inline void draw(RenderInstance* instance) //loadData()后才可以调用，否则报错
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

			void loadTextures(bool generate_minimap=true,bool release_image=true); //加载所有纹理到对应的纹理单元
			void useTextures(); //使用所有纹理（在不同纹理单元中）

			void loadData(); //加载顶点数据以及索引数据（如果有的话）进OpenGL，并设置好vertex_attribs对应的顶点属性格式

			template<typename ...Textures>
			inline void addTextures(Textures ... more_textures) //必须全部传入Texture2D类型的对象
			{
				textures.add(more_textures...);
			}
		};
	}
}

#endif//ANDROMEDA_GRAPHICS_RENDEROBJECT