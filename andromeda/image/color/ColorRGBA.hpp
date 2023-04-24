#ifndef ANDROMEDA_IMAGE_COLOR_COLORRGBA
#define ANDROMEDA_IMAGE_COLOR_COLORRGBA

#include "../Pixel.hpp"

namespace andromeda {
	namespace image {
		class Pixel;

		namespace color {
			class ColorRGBA
			{
			public:
				float r,g,b,a;

				ColorRGBA(int i_r,int i_g,int i_b,int i_a=0);
				ColorRGBA(float f_r=0,float f_g=0,float f_b=0,float f_a=0);
				ColorRGBA(Pixel pixel);

				Pixel toPixel();
				static ColorRGBA mixAlpha(ColorRGBA below,ColorRGBA above);

				inline float grayScale(float weight_r,float weight_g,float weight_b)
				{
					return (r*weight_r+g*weight_g+b*weight_b)/3.0;
				}
			};
		}
	}
}

#endif //ANDROMEDA_IMAGE_COLOR_COLORRGBA
