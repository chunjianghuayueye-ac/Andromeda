#ifndef ANDROMEDA_IMAGE_PIXEL
#define ANDROMEDA_IMAGE_PIXEL

#include <stdint.h>
#include "color/ColorRGBA.hpp"

namespace andromeda {
	namespace image {
		namespace color{
			class ColorRGBA;
		}

		class Pixel
		{
		public:
			unsigned char r,g,b,a;

			Pixel(int i_r=0,int i_g=0,int i_b=0,int i_a=0);
			Pixel(float f_r,float f_g,float f_b,float f_a=0);
			Pixel(color::ColorRGBA color);

			inline bool operator==(Pixel destpixel)
			{
				return (r==destpixel.r&&g==destpixel.g&&b==destpixel.b&&a==destpixel.a);
			}

			color::ColorRGBA getColorRGBA();

			inline uint32_t packToInt()
			{
				uint32_t pixel(r);
				return (((((pixel<<8)|g)<<8)|b)<<8)|a;
			}

			void setRGBA(int _r=-1,int _g=-1,int _b=-1,int _a=-1); //设置-1表示不变，0-255则改变为设置的值，超出这个范围的就取0或255

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
