#ifndef ANDROMEDA_UTIL_GLUTILS
#define ANDROMEDA_UTIL_GLUTILS

#include "../../lib/opengl/glad/glad.h"

namespace andromeda {
	namespace util {
		extern size_t getGLTypeSize(GLint type); //��ȡOpenGL���������ʹ�С
		/*ͨ���ַ�����д��ȡOpenGL�����ͣ����ַ�����ͷ����ƥ�䣬һ���ɹ����ۺ����ַ��������ݾ�ֱ�ӷ�������
		 * b��ub��2b��3b��4b��s��us��i��ui��f��d
		 */
		extern GLuint getGLType(const char* str);
	}
}

#endif//ANDROMEDA_UTIL_GLUTILS
