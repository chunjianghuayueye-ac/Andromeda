#include "Pixel.hpp"

#include "../color/ColorRGBA.hpp"
#include "RefPixel.hpp"
#include "../macros/Math.h"

using namespace anm2dr::color;
using namespace anm2dr::image;

Pixel::Pixel(int i_r,int i_g,int i_b,int i_a):
r(getPixelInt(i_r)),g(getPixelInt(i_g)),b(getPixelInt(i_b)),a(getPixelInt(i_a))
{}

Pixel::Pixel(float f_r,float f_g,float f_b,float f_a):
r(getPixelFloat(f_r)),g(getPixelFloat(f_g)),b(getPixelFloat(f_b)),a(getPixelFloat(f_a))
{}

Pixel::Pixel(const ColorRGBA &color):
r(getPixelFloat(color.r)),g(getPixelFloat(color.g)),b(getPixelFloat(color.b)),a(getPixelFloat(color.a))
{}

Pixel::Pixel(ColorRGBA &&color):
r(getPixelFloat(color.r)),g(getPixelFloat(color.g)),b(getPixelFloat(color.b)),a(getPixelFloat(color.a))
{}
