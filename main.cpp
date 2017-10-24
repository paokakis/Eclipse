#include <iostream>
#include "JNI\GraphicsManager.hpp"

int main()
{
	std::cout << "Hello!" << std::endl;
	GraphicsManager graphManager;
	
	graphManager.start();
	graphManager.loadShader(VERTEX_SHADER, FRAGMENT_SHADER);

	while (1)
	{
		graphManager.update();
	}

	return 0;
}