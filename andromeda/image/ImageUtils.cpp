#include "ImageUtils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../lib/image_codec/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../lib/image_codec/stb/stb_image_write.h"

#include "../macros/ImageProcess.h"
#include "../macros/Math.h"
#include "../color/ColorRGBA.hpp"
#include "RasterImage.hpp"
#include "Pixel.hpp"
#include "../util/RefcPointer.hpp"

//anm2dr::image
RasterImage& anm2dr::image::readImage(const char* image_path)
{
    RasterImage *image=new RasterImage();
    image->pixels=RefcPointer((Pixel*)stbi_load(image_path,&image->width,&image->height,nullptr,4));
    return *image;
}

void anm2dr::image::writeImage(const char* image_path,RasterImage& image)
{
    stbi_write_png(image_path,image.width,image.height,4,image.pixels.mem,4*image.width);
}

Pixel* anm2dr::image::generatePixelData(int width,int height,int r,int g,int b,int a)
{
    Pixel* data=(Pixel*)malloc(sizeof(Pixel)*width*height);
    fillPixelData(data,width,height,r,g,b,a);
    return data;
}

void anm2dr::image::fillPixelData(void *data,int height,int width,int r,int g,int b,int a)
{
    Pixel p(r,g,b,a);
    for(int i;i<width*height;++i)
        memcpy(data+4*i,&p,4);
}

int anm2dr::image::traversalPixels(RasterImage& image,traversal_func func,int x,int y,int stride_x,int stride_y,image_edge_options op)
{
    register int w=image.width,h=image.height,traversaled_count=0;
    register image_edge_options exec_flag_x,exec_flag_y=op;
    LOOP_Y:
        if(stride_x>w)
            return 0;
        exec_flag_x=op;
        LOOP_X:
            RefPixel pixel=image.getRefPixel(x,y);
            func(pixel);
            ++traversaled_count;
            ++x+=stride_x;
            if(x<w)
                goto LOOP_X;
        switch(exec_flag_x)
        {
        case IMAGE_EDGE_INCLUDE:
            x=w-1;
            exec_flag_x=IMAGE_EDGE_ABANDON;
            goto LOOP_X;
            break;
        case IMAGE_EDGE_ABANDON:
            break;
        }
        x=0;
        exec_flag_x=op;
        ++y+=stride_y;
        if(y<h)
            goto LOOP_Y;
        switch(exec_flag_y)
        {
        case IMAGE_EDGE_INCLUDE:
            y=h-1;
            exec_flag_y=IMAGE_EDGE_ABANDON;
            goto LOOP_Y;
            break;
        case IMAGE_EDGE_ABANDON:
            break;
        }
    return traversaled_count;
}

void anm2dr::image::convertRGBA32ToYUV444P(unsigned char* data,int width,int height,float br,float bg,float bb,unsigned char* Y,int linesize_Y,signed char* U,int linesize_U,signed char* V,int linesize_V)
{
    ColorRGBA back_color(getChannelFloat(br),getChannelFloat(bg),getChannelFloat(bb),1.0f);
    ColorRGBA this_color;
    for(int y=0; y<height; ++y)
    {
        for(int x=0; x<width; ++x)
        {
            this_color=ColorRGBA::mixAlpha(back_color,((Pixel*)data+y*width+x)->getColorRGBA());
            *(Y+y*linesize_Y+x)=getPixelLightFloat(0.299*this_color.r+0.587*this_color.g+0.114*this_color.b);
            *(U+y*linesize_U+x)=getPixelChrominaceFloat(-0.1687*this_color.r-0.3313*this_color.g+0.5*this_color.b);
            *(V+y*linesize_V+x)=getPixelChrominaceFloat(0.5*this_color.r-0.4187*this_color.g-0.0813*this_color.b);
        }
    }
}

void anm2dr::image::convertRGBA32ToYUV444P(unsigned char* data,int width,int height,float br,float bg,float bb,unsigned char* Y,signed char* U,signed char* V)
{
    anm2dr::image::convertRGBA32ToYUV444P(data,width,height,br,bg,bb,Y,width,U,width,V,width);
}

char** anm2dr::image::convertRGBA32ToYUV444P(unsigned char* data,int width,int height,float br,float bg,float bb)
{
    char** bytes_arr=(char**)malloc(3*sizeof(char*));
    unsigned char* Y=(unsigned char*)malloc(width*height);
    signed char* U=(signed char*)malloc(width*height);
    signed char* V=(signed char*)malloc(width*height);
    anm2dr::image::convertRGBA32ToYUV444P(data,width,height,br,bg,bb,Y,U,V);
    *bytes_arr=(char*)Y;
    *(bytes_arr+1)=(char*)U;
    *(bytes_arr+2)=(char*)V;
    return bytes_arr;
}

void anm2dr::image::convertRGBA32ToRGB24(unsigned char* data,int width,int height,float br,float bg,float bb,unsigned char* dest,int linesize)
{
    ColorRGBA back_color(getChannelFloat(br),getChannelFloat(bg),getChannelFloat(bb),1.0f);
    ColorRGBA this_color;
    for(int y=0; y<height; ++y)
    {
        for(int x=0; x<width; ++x)
        {
            this_color=ColorRGBA::mixAlpha(back_color,((Pixel*)(data+y*width+x))->getColorRGBA());
            *(dest+3*y*linesize+3*x)=getPixelFloat(this_color.r);
            *(dest+3*y*linesize+3*x+1)=getPixelFloat(this_color.g);
            *(dest+3*y*linesize+3*x+2)=getPixelFloat(this_color.b);
        }
    }
}

unsigned char* anm2dr::image::convertRGBA32ToRGB24(unsigned char* data,int width,int height,float br,float bg,float bb)
{
    unsigned char* bytes=(unsigned char*)malloc(3*width*height);
    anm2dr::image::convertRGBA32ToRGB24(data,width,height,br,bg,bb,bytes);
    return bytes;
}

void anm2dr::image::convertRGBA32ToRGB24(unsigned char* data,int width,int height,float br,float bg,float bb,unsigned char* dest)
{
    anm2dr::image::convertRGBA32ToRGB24(data,width,height,br,bg,bb,dest,width);
}

void anm2dr::image::convertYUV444PToRGBA32(int width,int height,unsigned char* Y,int linesize_Y,signed char* U,int linesize_U,signed char* V,int linesize_V,unsigned char* dest)
{
    for(int y=0; y<height; ++y)
    {
        for(int x=0; x<width; ++x)
        {
            unsigned char y=*(Y+y*linesize_Y+x);
            signed char u=*(U+y*linesize_U+x);
            signed char v=*(V+y*linesize_V+x);
            *(dest+4*y*width+4*x)=getChannelFloat(y+1.403*v);
            *(dest+4*y*width+4*x+1)=getChannelFloat(y-0.344*u-0.714*v);
            *(dest+4*y*width+4*x+2)=getChannelFloat(y+1.77*u);
            *(dest+4*y*width+4*x+3)=255;
        }
    }
}

void anm2dr::image::convertYUV444PToRGBA32(int width,int height,unsigned char* Y,signed char* U,signed char* V,unsigned char* dest)
{
    anm2dr::image::convertYUV444PToRGBA32(width,height,Y,width,U,width,V,width,dest);
}

void anm2dr::image::convertRGBA32Tou255YUV444P(unsigned char* data,int width,int height,float br,float bg,float bb,unsigned char* Y,int linesize_Y,unsigned char* U,int linesize_U,unsigned char* V,int linesize_V)
{
    ColorRGBA back_color(getChannelFloat(br),getChannelFloat(bg),getChannelFloat(bb),1.0f);
    Pixel this_pixel;
    for(int y=0; y<height; ++y)
    {
        for(int x=0; x<width; ++x)
        {
            this_pixel=ColorRGBA::mixAlpha(back_color,((Pixel*)data+y*width+x)->getColorRGBA()).toPixel();
            *(Y+y*linesize_Y+x)=0.299*this_pixel.r+0.587*this_pixel.g+0.114*this_pixel.b;
            *(U+y*linesize_U+x)=-0.1687*this_pixel.r-0.3313*this_pixel.g+0.5*this_pixel.b+128;
            *(V+y*linesize_V+x)=0.5*this_pixel.r-0.4187*this_pixel.g-0.0813*this_pixel.b+128;
        }
    }
}

unsigned char** anm2dr::image::convertRGBA32Tou255YUV444P(unsigned char* data,int width,int height,float br,float bg,float bb)
{
    unsigned char** bytes_arr=(unsigned char**)malloc(3*sizeof(char*));
    unsigned char* Y=(unsigned char*)malloc(width*height);
    unsigned char* U=(unsigned char*)malloc(width*height);
    unsigned char* V=(unsigned char*)malloc(width*height);
    anm2dr::image::convertRGBA32Tou255YUV444P(data,width,height,br,bg,bb,Y,U,V);
    *bytes_arr=Y;
    *(bytes_arr+1)=U;
    *(bytes_arr+2)=V;
    return bytes_arr;
}

void anm2dr::image::convertRGBA32Tou255YUV444P(unsigned char* data,int width,int height,float br,float bg,float bb,unsigned char* Y,unsigned char* U,unsigned char* V)
{
    anm2dr::image::convertRGBA32Tou255YUV444P(data,width,height,br,bg,bb,Y,width,U,width,V,width);
}

void anm2dr::image::convertu255YUV444PToRGBA32(int width,int height,unsigned char* Y,int linesize_Y,unsigned char* U,int linesize_U,unsigned char* V,int linesize_V,unsigned char* dest)
{
    for(int pos_y=0; pos_y<height; ++pos_y)
    {
        for(int pos_x=0; pos_x<width; ++pos_x)
        {
            unsigned char y=*(Y+pos_y*linesize_Y+pos_x);
            unsigned char u=*(U+pos_y*linesize_U+pos_x);
            unsigned char v=*(V+pos_y*linesize_V+pos_x);
            Pixel& this_pixel=*((Pixel*)dest+pos_y*width+pos_x);
            this_pixel.r=y+1.403*(v-128);
            this_pixel.g=y-0.343*(u-128)-0.714*(v-128);
            this_pixel.b=y+1.77*(u-128);
            this_pixel.a=255;
        }
    }
}

void anm2dr::image::convertu255YUV444PToRGBA32(int width,int height,unsigned char* Y,unsigned char* U,unsigned char* V,unsigned char* dest)
{
    anm2dr::image::convertu255YUV444PToRGBA32(width,height,Y,width,U,width,V,width,dest);
}

RasterImage& anm2dr::image::getRasterImageByu255YUV444P(int width,int height,unsigned char* Y,int linesize_Y,unsigned char* U,int linesize_U,unsigned char* V,int linesize_V)
{
    unsigned char* data=(unsigned char*)malloc(sizeof(Pixel)*width*height);
    convertu255YUV444PToRGBA32(width,height,Y,linesize_Y,U,linesize_U,V,linesize_V,data);
    RasterImage *img=new RasterImage(width,height,RefcPointer(data));
    return *img;
}
