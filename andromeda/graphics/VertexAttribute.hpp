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
			class VertexAttributeInfo //���Ե���Ϣ
			{
				friend class VertexAttribute;
			private:
				GLint index=-1;
				const char* name=nullptr;
				GLint type=GL_NONE;
				size_t type_size=0;
				bool normalized=false;
				int num=0;
				size_t offset=0; //�ڵ��������е�λ��(��Ϊ����������Գ��ȿ��Բ�һ������ͳһ���ֽ�����ʾλ��)

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
			//������OpenGL��VAO����VAO��������Ⱦ������һһ��Ӧ�ģ�����ֻ���涥�����Ը�ʽ
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

			/*����ָ�������ʽ���÷�"��������1:3f,��������2:n3f,��������3:2f...."
			 * �������Ʊ��ڼ��䣬˳���ʾ����������ţ���0��ʼ�����綥������1Ϊ����0����������2Ϊ����1...
			 * n��ʾnormalized���������ʾ���淶������������ֱ�ʾ���ݸ�����������1-4����������������ƣ��������ƶ�������
			 * b��ub��s��us��i��ui��f��d����������������Ӧ�ϸ�Сд
			 */
			void setAttribute(const char* attrib_str="position:3f,vertex_color:4f,texture_coord:2f");

			__attribute__((always_inline)) inline void load(GLuint vao) //��glBindBuffer()��glDraw*()ǰ���ã����ڰѶ������Ը�ʽװ�ؽ�ָ����VAO��
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
