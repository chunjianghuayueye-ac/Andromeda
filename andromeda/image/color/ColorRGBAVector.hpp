#ifndef ANDROMEDA_IMAGE_COLOR_COLORRGBAVECTOR
#define ANDROMEDA_IMAGE_COLOR_COLORRGBAVECTOR

#include "../declares/dec_color/ColorRGBA.h"
#include "../declares/dec_image/Pixel.h"

namespace andromeda{
    namespace color{
        class ColorRGBAVector
        {
        public:
            float rdefg,gdefb,length,alpha;

            ColorRGBAVector(float rdg=0,float gdb=0,float len=0,float a=0);
            ColorRGBAVector(const ColorRGBA &color);
            ColorRGBAVector(ColorRGBA &&color);
            ColorRGBAVector(const Pixel &pixel);
            ColorRGBAVector(Pixel &&pixel);
            ColorRGBAVector(const RefPixel &ref_pixel);
            ColorRGBAVector(RefPixel &&ref_pixel);

            void operator=(const ColorRGBA &color);
            void operator=(ColorRGBA &&color);
            ColorRGBA toColorRGBA();
            Pixel toPixel();
        };
    }
}

#include "../macros/Math.h"
#include "ColorRGBA.hpp"
#include "../image/Pixel.hpp"

using namespace anm2dr::color;
using namespace anm2dr::image;

inline void ColorRGBAVector::operator=(const ColorRGBA &color)
{
    rdefg=getRadianTangent(getChannelInt(color.r),getChannelInt(color.g));
    gdefb=getRadianTangent(getChannelInt(color.g),getChannelInt(color.b));
    length=getLength3DVector(getChannelInt(color.r),getChannelInt(color.g),getChannelInt(color.b));
    alpha=getChannelInt(color.a);
}

inline void ColorRGBAVector::operator=(ColorRGBA &&color)
{
    rdefg=getRadianTangent(getChannelInt(color.r),getChannelInt(color.g));
    gdefb=getRadianTangent(getChannelInt(color.g),getChannelInt(color.b));
    length=getLength3DVector(getChannelInt(color.r),getChannelInt(color.g),getChannelInt(color.b));
    alpha=getChannelInt(color.a);
}

inline ColorRGBA ColorRGBAVector::toColorRGBA()
{
    return ColorRGBA(getChannelRByRadianVector(rdefg,gdefb,length),getChannelGByRadianVector(rdefg,gdefb,length),getChannelBByRadianVector(rdefg,gdefb,length),alpha);
}

inline Pixel ColorRGBAVector::toPixel()
{
    return Pixel(ColorRGBA(getChannelRByRadianVector(rdefg,gdefb,length),getChannelGByRadianVector(rdefg,gdefb,length),getChannelBByRadianVector(rdefg,gdefb,length),alpha));
}

#endif //ANDROMEDA_IMAGE_COLOR_COLORRGBAVECTOR
