#include "Application.hpp"
#include <iostream>

using namespace andromeda::app;

Application::Application()
{
	Init_OpenGL:
	{
		glfwInit();
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
		glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	}
	new (this) Window("Andromeda Application");
}

void Application::initialize()
{

}

void Application::update()
{

}

void Application::terminate()
{
	glfwTerminate();
}

void Application::launch()
{
	isRunning=true;
	initialize();
	glfwMakeContextCurrent(window);
	while(isRunning)
	{
		update();
	}
	terminate();
}

void Application::exit()
{
	isRunning=false;
}

void Application::launchAsync()
{/*
 thread=new std::thread(launch);
 thread->detach();*/
}
