#ifndef ANM2DR_UTIL_UTILS
#define ANM2DR_UTIL_UTILS

#include <fstream>
#include <string.h>

namespace anm2dr{
    namespace util{

        extern void writeBytesToFile(const char* filename,void *data,long int length);
        extern void appendBytesToFile(const char* filename,void *data,long int length);
        extern void packBitsToBytes(unsigned char* bits,int bits_start_pos,unsigned char* bytes,int bytes_start_pos,long int bits_length);
        extern const char* int_to_string_dec(int num);
        extern const char* str_join(const char* str1,const char* str2);
        extern const char* str_join(const char* str1,int num);
        extern const char* str_join(int num,const char* str1);

        inline void clearFile(const char* file_name){
        	std::ofstream f(file_name,std::ios::out|std::ios::binary);
        	f.write(nullptr,0);
        	f.close();
        }

        template<typename T>
        __attribute__((always_inline)) void* memasgn(void* ptr,T &&value)
        {
        	return memcpy(ptr,&value,sizeof(T));
        }

        template<typename T>
        __attribute__((always_inline)) void* memasgn(void* ptr,T &value)
        {
            return memcpy(ptr,&value,sizeof(T));
        }
    }
}

#endif // ANM2DR_UTIL_UTILS
