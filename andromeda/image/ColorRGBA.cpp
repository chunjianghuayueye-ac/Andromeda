#include "ColorRGBA.hpp"

#include "RasterImage.hpp"
#include "Pixel.hpp"
#include "ColorChannel.hpp"

using namespace andromeda::image;

ColorRGBA::ColorRGBA(int i_r,int i_g,int i_b,int i_a) :
		r(getChannelInt(i_r)), g(getChannelInt(i_g)), b(getChannelInt(i_b)), a(getChannelInt(i_a))
{
}

ColorRGBA::ColorRGBA(float f_r,float f_g,float f_b,float f_a) :
		r(getChannelFloat(f_r)), g(getChannelFloat(f_g)), b(getChannelFloat(f_b)), a(getChannelFloat(f_a))
{
}

ColorRGBA::ColorRGBA(const Pixel pixel) :
		r(getChannelInt(pixel.r)), g(getChannelInt(pixel.g)), b(getChannelInt(pixel.b)), a(getChannelInt(pixel.a))
{
}

ColorRGBA::operator Pixel()
{
	return Pixel(r,g,b,a);
}

Pixel ColorRGBA::toPixel()
{
	return Pixel(r,g,b,a);
}

ColorRGBA& ColorRGBA::setRGBA(float _r,float _g,float _b,float _a) //设置-1表示不变，0-255则改变为设置的值，超出这个范围的就取0或255
{
	if(_r!=-1)
		r=getPixelFloat(_r);
	if(_g!=-1)
		g=getPixelFloat(_g);
	if(_b!=-1)
		b=getPixelFloat(_b);
	if(_a!=-1)
		a=getPixelFloat(_a);
	return *this;
}

ColorRGBA ColorRGBA::mixAlpha(ColorRGBA below,ColorRGBA above)
{
	ColorRGBA alpha_big,alpha_less;
	if(below.a>above.a)
	{
		alpha_big=below;
		alpha_less=above;
	}
	else if(below.a<above.a)
	{
		alpha_big=above;
		alpha_less=below;
	}
	else
	{
		if(below.a==1&&above.a==1)
			return above;
		return ColorRGBA((below.r+above.r)/2,(below.g+above.g)/2,(below.b+above.b)/2,below.a);
	}
	float unit=alpha_big.a;
	float rate=alpha_less.a/alpha_big.a;
	return ColorRGBA(alpha_less.r*rate*unit+alpha_big.r*(1-rate)*unit,alpha_less.g*rate*unit+alpha_big.g*(1-rate)*unit,alpha_less.b*rate*unit+alpha_big.b*(1-rate)*unit,unit);
}
