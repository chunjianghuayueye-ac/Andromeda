#include  "Window.hpp"
#include "../macros/Debug.h"

using namespace andromeda::app;

Window::Window(const char* title,int width,int height,bool isfullscreen,andromeda::image::color::ColorRGBA backColor_,GLFWmonitor* monitor_) :
		width(width), height(height), isFullScreen(isfullscreen), title(title), backColor(backColor_), monitor(monitor_)
{
	if(!title)
		PRINT_MESSAGE("Window name cannot be null")
	if(!(windowID=glfwCreateWindow(width,height,title,isfullscreen?monitor_:nullptr,nullptr)))
		PRINT_MESSAGE("Create Window failed")
}
