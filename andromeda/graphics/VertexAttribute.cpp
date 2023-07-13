#include "VertexAttribute.hpp"

#include "../util/StringUtils.hpp"

using namespace andromeda::graphics;
using namespace andromeda::util;

void VertexAttribute::setAttribute(const char* attrib_str)
{
	ArrayList<const char*>attrib_str_arr=split(attrib_str,","); //析构时自动释放内存
	size_t offset=0;
	for(int i=0;i<attrib_str_arr.getLength();++i)
	{
		VertexAttributeInfo attrib_info;
		attrib_info.index=i;
		ArrayList<const char*>attrib_str=split(attrib_str_arr[i],":");
		attrib_info.name=str_cpy(attrib_str[0]); //属性名称
		const char* attrib_data_info_str=attrib_str[1];
		register int pos=0;
		if(attrib_data_info_str[0]=='n') //需要规范化
		{
			pos=1;
			attrib_info.normalized=true;
		}
		attrib_info.num=char_to_int_0_to_9(attrib_data_info_str[pos]);
		attrib_info.type=getGLType(attrib_data_info_str+(++pos));
		attrib_info.type_size=getGLTypeSize(attrib_info.type);
		attrib_info.offset=offset;
		attribs.add(attrib_info);
		offset+=attrib_info.num*attrib_info.type_size; //下一个顶点（如果存在）的初始位置
	}
	free((void*)attrib_str_arr[0]);
	vertex_size=offset;
}

VertexAttribute::VertexAttributeInfo VertexAttribute::getVertexAttributeInfo(const char* attrib_name)
{
	for(int i=0;i<attribs.getLength();++i)
		if(strcmp(attribs[i].name,attrib_name))
			return attribs[i];
	return VertexAttribute::VertexAttributeInfo(); //无效查询
}

VertexAttribute* VertexAttribute::default_pct_vertex_attributes=nullptr;
VertexAttribute* VertexAttribute::default_pt_vertex_attributes=nullptr;
