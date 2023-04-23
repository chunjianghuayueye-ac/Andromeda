#include "RasterImage.hpp"
#include <string.h>
#include "../macros/ImageProcess.h"
#include "ImageUtils.hpp"
#include "Pixel.hpp"
#include "../color/ColorRGBA.hpp"

using namespace andromeda::image;
using namespace andromeda::image::color;

RasterImage::RasterImage(int img_width,int img_height,RefcPointer data):
    height(img_height),width(img_width),pixels(data)
{}

RasterImage RasterImage::copyRasterImage()
{
    int len=height*width;
    Pixel* cpy=(Pixel*)malloc(sizeof(Pixel)*len);
    memcpy(cpy,pixels.mem,sizeof(Pixel)*len);
    return RasterImage(width,height,RefcPointer(cpy));
}

void RasterImage::allocate()
{
    pixels=RefcPointer((Pixel*)malloc(width*height*sizeof(Pixel)));
}

void RasterImage::fillRect(int start_x,int start_y,int end_x,int end_y,Pixel pixel)
{
    RasterImage &this_tmage=*this;
    startTraversalPixelsInArea(this_tmage,start_x,start_y,end_x,end_y,0,0,IMAGE_EDGE_INCLUDE)
    this_pixel_in_area=pixel;
    endTraversalPixelsInArea(this_tmage,start_x,start_y,end_x,end_y,0,0,IMAGE_EDGE_INCLUDE)
}

void RasterImage::fillAll(Pixel pixel)
{
    fillRect(0,0,width-1,height-1,pixel);
}

void RasterImage::mix(RasterImage &img,int pos_x,int pos_y)
{
    int traversal_width=pos_x+img.width>width?width-pos_x:img.width;
    int traversal_height=pos_y+img.height>height?height-pos_y:img.height;
    startTraversalPixelsInArea(img,0,0,traversal_width,traversal_height,0,0,IMAGE_EDGE_INCLUDE)
    Pixel &pixel_in_src_image=this->getPixel(pos_x+absolute_x_in_area,pos_y+absolute_y_in_area);
    pixel_in_src_image=ColorRGBA::mixAlpha(pixel_in_src_image.getColorRGBA(),this_pixel_in_area.getColorRGBA()).toPixel();
    endTraversalPixelsInArea(img,0,0,traversal_width,traversal_height,0,0,IMAGE_EDGE_INCLUDE)
}

void RasterImage::mulAlphaFactor(float factor)
{
    RasterImage &this_image=*this;
    startTraversalPixels(this_image,0,0,0,0,IMAGE_EDGE_INCLUDE)
    this_pixel_in_image.a*=factor;
    endTraversalPixels(this_image,0,0,0,0,IMAGE_EDGE_INCLUDE)
}

RasterImage& RasterImage::cutImage(RasterImage& src_img,int start_x,int start_y,int end_x,int end_y)
{
    RasterImage* result=new RasterImage(end_x-start_x,end_y-start_y);
    result->allocate();
    startTraversalPixelsInArea(src_img,start_x,start_y,end_x,end_y,0,0,IMAGE_EDGE_INCLUDE)
    result->setPixel(relative_x_in_area,relative_y_in_area,this_pixel_in_area);
    endTraversalPixelsInArea(src_img,start_x,start_y,end_x,end_y,0,0,IMAGE_EDGE_INCLUDE)
    return *result;
}

inline Pixel& RasterImage::getPixel(int x,int y)
{
	return *(Pixel*)(pixels.mem+width*y+x);
}

inline Pixel* RasterImage::getPixelDataPtr()
{
	return (Pixel*)pixels.mem;
}

inline RefcPointer RasterImage::getPixelDataMem()
{
	return pixels;
}

inline void RasterImage::setPixelData(Pixel* data,int img_width,int img_height,image_size_options op)
{
	switch(op)
	{
		case IMAGE_SIZE_FIXED:

			break;
		case IMAGE_SIZE_FIT:

			break;
	}
}

inline RefPixel RasterImage::getRefPixel(int x,int y)
{
	return RefPixel(this,x,y);
}

inline void RasterImage::setPixel(int x,int y,Pixel new_pixel)
{
	*(Pixel*)(pixels.mem+width*y+x)=new_pixel;
}

inline void RasterImage::setPixel(int x,int y,RefPixel new_pixel)
{
	*(Pixel*)(pixels.mem+width*y+x)=new_pixel;
}

inline RasterImage& RasterImage::fromFile(const char* path)
{
	return anm2dr::image::readImage(path);
}
