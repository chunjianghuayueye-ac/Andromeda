#ifndef ANDROMEDA_IMAGE_PIXEL
#define ANDROMEDA_IMAGE_PIXEL

#include <stdint.h>
#include "ColorRGBA.hpp"
#include "ColorChannel.hpp"
#include "../macros/Math.h"

namespace andromeda {
	namespace image {
		class ColorRGBA;
		class Pixel
		{
		public:
			unsigned char r,g,b,a;

			Pixel(int i_r=0,int i_g=0,int i_b=0,int i_a=0);
			Pixel(float f_r,float f_g,float f_b,float f_a=0);
			Pixel(ColorRGBA color);

			inline bool operator==(Pixel destpixel)
			{
				return (r==destpixel.r&&g==destpixel.g&&b==destpixel.b&&a==destpixel.a);
			}

			ColorRGBA toColorRGBA();

			inline uint32_t packToInt()
			{
				uint32_t pixel(r);
				return (((((pixel<<8)|g)<<8)|b)<<8)|a;
			}

			inline Pixel& setRGBA(Pixel pixel)
			{
				memcpy(this,&pixel,sizeof(Pixel));
				return *this;
			}

			inline Pixel& setRGBA(ColorChannel color_ch,float color_value)
			{
				*((int*)this+color_ch)=getChannelInt(color_value);
				return *this;
			}

			inline int getRGBA(ColorChannel color_ch)
			{
				return *((int*)this+color_ch);
			}

			Pixel& setRGBA(int _r=-1,int _g=-1,int _b=-1,int _a=-1); //����-1��ʾ���䣬0-255��ı�Ϊ���õ�ֵ�����������Χ�ľ�ȡ0��255

			inline Pixel& mulFactor(float f)
			{
				r*=f;
				g*=f;
				b*=f;
				return *this;
			}
		};
	}
}

#endif // ANDROMEDA_IMAGE_PIXEL
