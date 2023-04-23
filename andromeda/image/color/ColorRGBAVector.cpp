#include "ColorRGBAVector.hpp"

#include "../macros/Math.h"
#include "ColorRGBA.hpp"
#include "../image/Pixel.hpp"

using namespace andromeda::image::color;
using namespace andromeda::image;

ColorRGBAVector::ColorRGBAVector(float rdg,float gdb,float len,float a):
rdefg(rdg),gdefb(gdb),length(len),alpha(a)
{}

ColorRGBAVector::ColorRGBAVector(const ColorRGBA &color):
rdefg(getRadianTangent(color.r,color.g)),gdefb(getRadianTangent(color.g,color.b)),length(getLength3DVector(color.r,color.g,color.b)),alpha(color.a)
{}

ColorRGBAVector::ColorRGBAVector(ColorRGBA &&color):
rdefg(getRadianTangent(color.r,color.g)),gdefb(getRadianTangent(color.g,color.b)),length(getLength3DVector(color.r,color.g,color.b)),alpha(color.a)
{}

ColorRGBAVector::ColorRGBAVector(const Pixel &pixel):
rdefg(getRadianTangent(getChannelInt(pixel.r),getChannelInt(pixel.g))),gdefb(getRadianTangent(getChannelInt(pixel.g),getChannelInt(pixel.b))),length(getLength3DVector(getChannelInt(pixel.r),getChannelInt(pixel.g),getChannelInt(pixel.b))),alpha(getChannelInt(pixel.a))
{}

ColorRGBAVector::ColorRGBAVector(Pixel &&pixel):
rdefg(getRadianTangent(getChannelInt(pixel.r),getChannelInt(pixel.g))),gdefb(getRadianTangent(getChannelInt(pixel.g),getChannelInt(pixel.b))),length(getLength3DVector(getChannelInt(pixel.r),getChannelInt(pixel.g),getChannelInt(pixel.b))),alpha(getChannelInt(pixel.a))
{}

ColorRGBAVector::ColorRGBAVector(const RefPixel &ref_pixel):
rdefg(getRadianTangent(getChannelInt(ref_pixel.pixel->r),getChannelInt(ref_pixel.pixel->g))),gdefb(getRadianTangent(getChannelInt(ref_pixel.pixel->g),getChannelInt(ref_pixel.pixel->b))),length(getLength3DVector(getChannelInt(ref_pixel.pixel->r),getChannelInt(ref_pixel.pixel->g),getChannelInt(ref_pixel.pixel->b))),alpha(getChannelInt(ref_pixel.pixel->a))
{}

ColorRGBAVector::ColorRGBAVector(RefPixel &&ref_pixel):
rdefg(getRadianTangent(getChannelInt(ref_pixel.getR()),getChannelInt(ref_pixel.getG()))),gdefb(getRadianTangent(getChannelInt(ref_pixel.getG()),getChannelInt(ref_pixel.getB()))),length(getLength3DVector(getChannelInt(ref_pixel.getR()),getChannelInt(ref_pixel.getG()),getChannelInt(ref_pixel.getB()))),alpha(getChannelInt(ref_pixel.getA()))
{}
