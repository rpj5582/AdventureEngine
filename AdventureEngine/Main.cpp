#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "FreeImage.lib")

#include "Engine.h"

int main(int argc, char* argv)
{
	AdventureEngine::Engine engine;
	engine.start();

	return 0;
}