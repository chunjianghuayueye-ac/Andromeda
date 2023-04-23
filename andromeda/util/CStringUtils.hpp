#ifndef ANM2DR_UTIL_CSTRINGUTILS
#define ANM2DR_UTIL_CSTRINGUTILS

namespace anm2dr{
    namespace util{
        inline char int_0_to_9_to_char(int num)
        {
            return (char)(num+48);
        }
        const char* int_to_string_dec(int num);

        const char* str_join(const char* str1,const char* str2);
        const char* str_join(const char* str1,int num);
        const char* str_join(int num,const char* str1);

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

#endif // ANM2DR_UTIL_CSTRINGUTILS
