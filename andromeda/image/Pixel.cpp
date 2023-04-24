#include "Pixel.hpp"

#include "color/ColorRGBA.hpp"
#include "../macros/Math.h"

using namespace andromeda::image::color;
using namespace andromeda::image;

Pixel::Pixel(int i_r,int i_g,int i_b,int i_a) :
		r(getPixelInt(i_r)), g(getPixelInt(i_g)), b(getPixelInt(i_b)), a(getPixelInt(i_a))
{
}

Pixel::Pixel(float f_r,float f_g,float f_b,float f_a) :
		r(getPixelFloat(f_r)), g(getPixelFloat(f_g)), b(getPixelFloat(f_b)), a(getPixelFloat(f_a))
{
}

Pixel::Pixel(ColorRGBA color) :
		r(getPixelFloat(color.r)), g(getPixelFloat(color.g)), b(getPixelFloat(color.b)), a(getPixelFloat(color.a))
{
}

ColorRGBA Pixel::getColorRGBA()
{
	return ColorRGBA(r,g,b,a);
}

void Pixel::setRGBA(int _r,int _g,int _b,int _a) //����-1��ʾ���䣬0-255��ı�Ϊ���õ�ֵ�����������Χ�ľ�ȡ0��255
{
	if(_r!=-1)
		r=getPixelInt(_r);
	if(_g!=-1)
		g=getPixelInt(_g);
	if(_b!=-1)
		b=getPixelInt(_b);
	if(_a!=-1)
		a=getPixelInt(_a);
}
