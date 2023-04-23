#ifndef ANDROMEDA_APP_WINDOW
#define ANDROMEDA_APP_WINDOW

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace andromeda
{
namespace app
{
class Window
{
private:
	GLFWwindow* windowID;
	int width, height;
	bool isFullScreen;
	const char* title;

public:
	Window();
	Window(int width, int height, const char* title, bool isfullscreen);
	Window(int width, int height, const char* title);
	Window(const char* title);

	inline operator GLFWwindow*()
	{
		return windowID;
	}

	inline int getWidth(void)
	{
		return width;
	}

	inline int getHeight(void)
	{
		return height;
	}

	inline bool isWindowFullScreen(void)
	{
		return isFullScreen;
	}

	inline void setFullScreen(bool isFullScreen_)
	{
		isFullScreen = isFullScreen_;
		glfwSetWindowMonitor(windowID,
				isFullScreen_ ? glfwGetPrimaryMonitor() : nullptr, 0, 0, width,
				height, GLFW_DONT_CARE);
	}

	inline void setWindowSize(int width_, int height_)
	{
		width = width_;
		height = height_;
		glfwSetWindowSize(windowID, width_, height_);
	}
};
}
}

#endif//ANDROMEDA_APP_WINDOW
