#ifndef ANDROMEDA_IO_IOUTILS
#define ANDROMEDA_IO_IOUTILS

#include <fstream>
#include <string.h>

namespace andromeda {
	namespace io {
		extern unsigned char* readBytesFromFile(const char* filename,size_t *data_length=nullptr,long long int read_length=-1,size_t reserve_length=0); //data_length��¼�������鳤�ȣ�read_length<0���ȡȫ�����ݣ�reserve_lengthΪ�ڴ汣���ֽ���
		extern const char* readStringFromFile(const char* filename,size_t *str_length=nullptr,long long int read_length=-1,size_t reserve_length=0); //str_length��¼�����ַ������Ȳ���'\0'��read_length=<0���ȡȫ�����ݣ�reserve_lengthΪ�ڴ汣���ֽ���
		extern void writeBytesToFile(const char* filename,void *data,size_t length);
		extern void appendBytesToFile(const char* filename,void *data,size_t length);
		inline void clearFile(const char* file_name)
		{
			std::ofstream f(file_name,std::ios::out|std::ios::binary);
			f.write(nullptr,0);
			f.close();
		}
	}
}

#endif // ANDROMEDA_IO_IOUTILS
