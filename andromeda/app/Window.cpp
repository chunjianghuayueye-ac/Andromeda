#include  "Window.hpp"

using namespace andromeda::app;

Window::Window(int width,int height,const char* title,bool isfullscreen,GLFWmonitor* monitor_) :
		width(width), height(height), isFullScreen(isfullscreen), title(title), monitor(monitor_)
{
	windowID=glfwCreateWindow(width,height,title,isfullscreen?monitor_:nullptr,nullptr);
	glViewport(0,0,width,height);
}

Window::Window(int width,int height,const char* title) :
		Window(width,height,title,false)
{
}

Window::Window(const char* title) :
		Window(800,600,title,false)
{
}

Window::Window() :
		Window(800,600,nullptr,false)
{
}

