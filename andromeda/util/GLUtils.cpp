#include "GLUtils.hpp"

#include <string.h>

using namespace andromeda::util;

size_t andromeda::util::getGLTypeSize(GLint type)
{
	switch(type)
	{
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		return sizeof(char);
	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
		return sizeof(short int);
	case GL_INT:
	case GL_UNSIGNED_INT:
		return sizeof(int);
	case GL_FLOAT:
		return sizeof(float);
	case GL_DOUBLE:
		return sizeof(double);
	case GL_2_BYTES:
		return sizeof(char)*2;
	case GL_3_BYTES:
		return sizeof(char)*3;
	case GL_4_BYTES:
		return sizeof(char)*4;
	}
	return 0; //����0��ʾ��ѯʧ�ܣ������type����OpenGL����������
}

GLuint andromeda::util::getGLType(const char* str)
{
	bool at_least_2_chars=strlen(str)>1;
	switch(*str)
	{
	case 'b':
		return GL_BYTE;
	case 's':
		return GL_SHORT;
	case 'i':
		return GL_INT;
	case 'f':
		return GL_FLOAT;
	case 'd':
		return GL_DOUBLE;
	case 'u':
		if(at_least_2_chars)
			switch(*(str+1))
			{
			case 'b':
				return GL_UNSIGNED_BYTE;
			case 's':
				return GL_UNSIGNED_SHORT;
			case 'i':
				return GL_UNSIGNED_INT;
			}
		break;
	case '2':
		if(at_least_2_chars)
			if(*(str+1)=='b')
				return GL_2_BYTES;
		break;
	case '3':
		if(at_least_2_chars)
			if(*(str+1)=='b')
				return GL_3_BYTES;
		break;
	case '4':
		if(at_least_2_chars)
			if(*(str+1)=='b')
				return GL_4_BYTES;
		break;
	}
	return GL_NONE;
}
