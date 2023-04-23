#ifndef ANM2DR_IMAGE_RASTERIMAGE
#define ANM2DR_IMAGE_RASTERIMAGE

#include "Pixel.hpp"

namespace andromeda {
	namespace image {
		class RasterImage
		{

				friend RasterImage& anm2dr::image::readImage(const char* image_path);
				friend void anm2dr::image::writeImage(const char* image_path,RasterImage& image);

			public:
				int height,width;

				RasterImage(int img_width=0,int img_height=0,Pixel* data=malloc(0));

				RasterImage copyRasterImage();

				Pixel* getPixelDataPtr();
				void setPixelData(Pixel* data,int img_width,int img_height,image_size_options op);
				Pixel& getPixel(int x,int y);
				void setPixel(int x,int y,Pixel new_pixel);
				void setPixel(int x,int y,RefPixel new_pixel);
				void allocate();
				static RasterImage& fromFile(const char* path);

				RefPixel getRefPixel(int x,int y);

				//Operations
				void fillRect(int start_x,int start_y,int end_x,int end_y,Pixel pixel);
				void fillAll(Pixel pixel);

				void mix(RasterImage &img,int pos_x,int pos_y);
				void mulAlphaFactor(float factor);

				static RasterImage& cutImage(RasterImage& src_img,int start_x,int start_y,int end_x,int end_y);
			protected:
				Pixel* pixels;
		};
	}
}

#endif // ANM2DR_IMAGE_RASTERIMAGE
