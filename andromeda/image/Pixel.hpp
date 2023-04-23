#ifndef ANM2DR_IMAGE_PIXEL
#define ANM2DR_IMAGE_PIXEL

#include <stdint.h>

#include "../declares/dec_color/ColorRGBA.h"
#include "../declares/dec_image/RefPixel.h"

using namespace anm2dr::color;
using namespace anm2dr::image;

namespace anm2dr{
    namespace image{
        class Pixel
        {
        public:
            unsigned char r,g,b,a;

            Pixel(int i_r=0,int i_g=0,int i_b=0,int i_a=0);
            Pixel(float f_r,float f_g,float f_b,float f_a=0);
            Pixel(const ColorRGBA &color);
            Pixel(ColorRGBA &&color);

            void operator=(const RefPixel &new_pixel);
            void operator=(RefPixel &&new_pixel);

            bool operator==(const Pixel &destpixel);
            bool operator==(Pixel &&destpixel);
            bool operator==(const RefPixel &destpixel);
            bool operator==(RefPixel &&destpixel);

            ColorRGBA getColorRGBA();

            Pixel& mulFactor(float f);

            uint32_t packToInt();
            void setRGBA(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a);
        };
    }
}

#include "../color/ColorRGBA.hpp"
#include "RefPixel.hpp"

using namespace anm2dr::image;
using namespace anm2dr::color;

inline void Pixel::operator=(RefPixel &&new_pixel)
{
    r=new_pixel.pixel->r;
    g=new_pixel.pixel->g;
    b=new_pixel.pixel->b;
    a=new_pixel.pixel->a;
}

inline void Pixel::operator=(const RefPixel &new_pixel)
{
    r=new_pixel.pixel->r;
    g=new_pixel.pixel->g;
    b=new_pixel.pixel->b;
    a=new_pixel.pixel->a;
}

inline bool Pixel::operator==(const Pixel &destpixel)
{
    return (r==destpixel.r&&g==destpixel.g&&b==destpixel.b&&a==destpixel.a);
}

inline bool Pixel::operator==(Pixel &&destpixel)
{
    return (r==destpixel.r&&g==destpixel.g&&b==destpixel.b&&a==destpixel.a);
}

inline bool Pixel::operator==(const RefPixel &destpixel)
{
    return (r==destpixel.pixel->r&&g==destpixel.pixel->g&&b==destpixel.pixel->b&&a==destpixel.pixel->a);
}

inline bool Pixel::operator==(RefPixel &&destpixel)
{
    return (r==destpixel.pixel->r&&g==destpixel.pixel->g&&b==destpixel.pixel->b&&a==destpixel.pixel->a);
}

inline ColorRGBA Pixel::getColorRGBA()
{
    return ColorRGBA(r,g,b,a);
}

inline uint32_t Pixel::packToInt()
{
    uint32_t pixel(r);
    return (((((pixel<<8)|g)<<8)|b)<<8)|a;
}

inline void Pixel::setRGBA(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a)
{
    r=_r;
    g=_g;
    b=_b;
    a=_a;
}

inline Pixel& Pixel::mulFactor(float f)
{
	r*=f;
	g*=f;
	b*=f;
	return *this;
}

#endif // ANM2DR_IMAGE_PIXEL
