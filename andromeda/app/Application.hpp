#ifndef ANDROMEDA_APP_APPLICATION
#define ANDROMEDA_APP_APPLICATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window.hpp"

namespace andromeda {
	namespace app {
		class Application
		{
			private:
				Window window;
				bool isRunning=false;

			protected:
				void initialize();
				void terminate();
				void update();
			public:
				Application();

				void launch();
				void launchAsync();

				void exit();

				inline int getWindowHeight(void)
				{
					return window.getHeight();
				}

				inline int getWindowWidth(void)
				{
					return window.getWidth();
				}

				inline bool isFullScreen(void)
				{
					return window.isWindowFullScreen();
				}

				inline void setFullScreen(bool isFullScreen){
					window.setFullScreen(isFullScreen);
				}

				inline void setWindowSize(int width,int height)
				{
					window.setWindowSize(width,height);
				}
		};
	}
}

#endif//ANDROMEDA_APP_APPLICATION
