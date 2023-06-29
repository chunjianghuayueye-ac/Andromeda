#include "macros/Debug.h"
#include "../lib/opengl/GLFW/glfw3.h"
#include "../lib/portaudio/portaudio.h"

namespace andromeda {
	bool use_opengl=true;
	bool use_portaudio=true;

	void _glfw_error_print(int err_code,const char* description)
	{
		PRINT_MESSAGE("GLFW Error Code:",err_code,"\nDescription:",description)
	}

	void _glfw_framebuffer_size_callback(GLFWwindow* window,int width,int height)
	{
		glViewport(0,0,width,height);
	}
}

using namespace andromeda;

__attribute__((constructor)) void _init_lib()
{
	if(use_opengl)
	{
		if(!glfwInit())
			PRINT_MESSAGE("Initialize GLFW failed.")
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
		glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		glfwSetErrorCallback(andromeda::_glfw_error_print);
	}
	if(use_portaudio)
	{
		auto err=Pa_Initialize();
		if(err!=paNoError)
			PRINT_MESSAGE("Initialize PortAudio failed. Error: ",Pa_GetErrorText(err))
	}
}

__attribute__((destructor)) void _term_lib()
{
	if(use_opengl)
	{
		glfwTerminate();
	}
	if(use_portaudio)
	{
		auto err=Pa_Terminate();
		if(err!=paNoError)
			std::cout<<"Terminate PortAudio failed. Error: "<<Pa_GetErrorText(err)<<std::endl;
	}
}

