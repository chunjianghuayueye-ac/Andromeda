#ifndef ANDROMEDA_GRAPHICS_VERTEXATTRIBUTE
#define ANDROMEDA_GRAPHICS_VERTEXATTRIBUTE

#include "../../lib/opengl/glad/glad.h"
#include "../util/ArrayList.hpp"
#include "../util/GLUtils.hpp"

namespace andromeda {
	namespace graphics {
		class VertexAttribute
		{
		public:
			class VertexAttributeInfo //属性的信息
			{
				friend class VertexAttribute;
			private:
				GLint index=-1;
				const char* name=nullptr;
				GLint type=GL_NONE;
				size_t type_size=0;
				bool normalized=false;
				int num=0;
				size_t offset=0; //在单个顶点中的位移(因为顶点各个属性长度可以不一样，故统一用字节数表示位移)

			public:
				VertexAttributeInfo()=default;

				VertexAttributeInfo(GLint index,const char* name,GLint type,bool normalized,int num,size_t offset) :
						index(index), name(name), type(type), normalized(normalized), num(num), offset(offset)
				{
					type_size=andromeda::util::getGLTypeSize(type);
				}

				inline GLint getIndex()
				{
					return index;
				}

				inline GLint getType()
				{
					return type;
				}

				inline bool getNormalized()
				{
					return normalized;
				}

				inline int getNum()
				{
					return num;
				}

				inline size_t getOffset()
				{
					return offset;
				}

				inline const char* getName()
				{
					return name;
				}
			};
		private:
			//不储存OpenGL的VAO对象，VAO对象与渲染对象是一一对应的，本类只储存顶点属性格式
			size_t vertex_size=0;
			andromeda::util::ArrayList<VertexAttributeInfo> attribs;
			static VertexAttribute* default_pct_vertex_attributes;//p=position,c=color,t=texture coord
			static VertexAttribute* default_pt_vertex_attributes;//p=position,t=texture coord
		public:
			VertexAttribute()=default;

			VertexAttribute(const char* attrib_info)
			{
				setAttribute(attrib_info);
			}

			/*用于指定顶点格式，用法"顶点名称1:3f,顶点名称2:n3f,顶点名称3:2f...."
			 * 顶点名称便于记忆，顺序表示顶点属性序号，从0开始，例如顶点名称1为属性0，顶点名称2为属性1...
			 * n表示normalized，不加则表示不规范化。后面加数字表示数据个数（必须是1-4），最后是类型名称，类型名称定义如下
			 * b，ub，s，us，i，ui，f，d，除顶点名称其余应严格小写
			 */
			void setAttribute(const char* attrib_str="position:3f,vertex_color:4f,texture_coord:2f");

			__attribute__((always_inline)) inline void load(GLuint vao) //在glBindBuffer()后、glDraw*()前调用，用于把顶点属性格式装载进指定的VAO中
			{
				glBindVertexArray(vao);
				for(int i=0;i<attribs.getLength();++i)
				{
					VertexAttribute::VertexAttributeInfo& attrib=attribs[i];
					glVertexAttribPointer(attrib.index,attrib.num,attrib.type,attrib.normalized,vertex_size,(void*)(attrib.offset));
					glEnableVertexAttribArray(attrib.index);
				}
			}

			VertexAttributeInfo getVertexAttributeInfo(const char* attrib_name);

			inline VertexAttributeInfo getVertexAttributeInfo(int index)
			{
				return attribs[index];
			}

			inline size_t getVertexSize()
			{
				return vertex_size;
			}

			static inline VertexAttribute& getDefaultPCTVertexAttributes()
			{
				if(!default_pct_vertex_attributes)
					default_pct_vertex_attributes=new VertexAttribute("position:3f,vertex_color:4f,texture_coord:2f");
				return *default_pct_vertex_attributes;
			}

			static inline VertexAttribute& getDefaultPTVertexAttributes()
			{
				if(!default_pt_vertex_attributes)
					default_pt_vertex_attributes=new VertexAttribute("position:3f,texture_coord:2f");
				return *default_pt_vertex_attributes;
			}
		};
	}
}

#endif//ANDROMEDA_GRAPHICS_VERTEXATTRIBUTE
