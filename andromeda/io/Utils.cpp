#include <fstream>
#include <iostream>
#include <string.h>


namespace anm2dr{
    namespace util{
        void writeBytesToFile(const char* filename,void *data,long int length)
        {
            std::ofstream file(filename,std::ios::out|std::ios::binary);
            file.write((const char*)data,length);
            file.close();
        }

        void appendBytesToFile(const char* filename,void *data,long int length)
        {
            std::ofstream file(filename,std::ios::app|std::ios::binary);
            file.write((const char*)data,length);
            file.close();
        }

        void packBitsToBytes(unsigned char* bits,int bits_start_pos,unsigned char* bytes,int bytes_start_pos,long int bits_length)
        {
             unsigned char byte;
             int bit_count,byte_index=0,bit_index=0;
            while(bit_index<bits_length)
            {
                bit_count=0;
                byte=0;
                while(bit_count<8&&bit_index<bits_length)
                {
                    byte|=(*(bits+bits_start_pos+bit_index)<<(8-bit_count-1));
                    ++bit_count;
                    ++bit_index;
                }
                *(bytes+bytes_start_pos+byte_index)=byte;
                ++byte_index;
            }
        }

        const char* int_to_string_dec(int num)
        {
            char* str=nullptr;
            int num_len=1;
            int limit=0;
            char num_ch='+';
            if(num<0)
            {
                num_ch='-';
                num=-num;
                ++limit;
            }
            register int tmp=num;
            while(tmp>=10)
            {
                tmp/=10;
                ++num_len;
            }
            int pos=num_len;
            switch(num_ch)
            {
            case '-':
                str=new char[num_len+2];
                str[0]='-';
                str[num_len+1]='\0';
                break;
            case '+':
                str=new char[num_len+1];
                str[num_len]='\0';
                --pos;
                break;
            }
            for(tmp=num;pos>=limit;tmp/=10,--pos)
                str[pos]='0'+(tmp%10);
            return (const char*)str;
        }

        const char* str_join(const char* str1,const char* str2)
        {
            register int len=strlen(str1)+strlen(str2)+1;
            char* str=new char[len];
            str[len-1]='\0';
            strcpy(str,str1);
            strcpy(str+strlen(str1),str2);
            return (const char*)str;
        }

        const char* str_join(const char* str1,int num)
        {
            return str_join(str1,int_to_string_dec(num));
        }

        const char* str_join(int num,const char* str1)
        {
            return str_join(int_to_string_dec(num),str1);
        }
    }
}
