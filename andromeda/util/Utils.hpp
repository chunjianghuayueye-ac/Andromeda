#ifndef ANDROMEDA_UTIL_UTILS
#define ANDROMEDA_UTIL_UTILS

namespace andromeda {
	namespace util {

		extern void packBitsToBytes(unsigned char* bits,int bits_start_pos,unsigned char* bytes,int bytes_start_pos,long int bits_length);
		extern const char* int_to_string_dec(int num);

		extern const char* str_join(const char* str1,const char* str2);
		extern const char* str_join(const char* str1,int num);
		extern const char* str_join(int num,const char* str1);
		extern const char* str_join(const char* str1,char ch);
		extern const char* str_join(char ch,const char* str1);
		extern const char* str_join(bool b,const char* str1);
		extern const char* str_join(const char* str1,bool b);

		__attribute__((always_inline)) inline const char* str_join(const char* str)
		{
			return str;
		}

		__attribute__((always_inline)) inline const char* str_join(int num)
		{
			return int_to_string_dec(num);
		}

		__attribute__((always_inline)) inline const char* str_join(bool b)
		{
			return b?"true":"false";
		}

		__attribute__((always_inline)) inline const char* str_join(char ch)
		{
			return (const char*)new char[1]{ch};
		}

		template<typename T>
		__attribute__((always_inline)) inline void* memasgn(void* ptr,T &&value)
		{
			return memcpy(ptr,&value,sizeof(T));
		}

		template<typename T>
		__attribute__((always_inline)) void* memasgn(void* ptr,T &value)
		{
			return memcpy(ptr,&value,sizeof(T));
		}
		inline char int_0_to_9_to_char(int num)
		{
			return (char)(num+48);
		}
		const char* int_to_string_dec(int num);

		template<typename T1>
		const char* str_join(T1 str1)
		{
			return str_join(str1);
		}
		template<typename T1,typename T2>
		const char* str_join(T1 str1,T2 str2)
		{
			return str_join(str1,str2);
		}
		template<typename T,typename ...Ts>
		const char* str_join(T str,Ts ...strs)
		{
			return str_join(str,str_join(strs...));
		}
	}
}

#endif // ANDROMEDA_UTIL_UTILS
