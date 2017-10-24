#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <glfw3.h>

class Keyboard
{
public:
	static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);

	static bool keyDown(int key);
	static bool keyUp(int key);
	static bool key(int key);
private:
	static bool keys[];
	static bool keysUp[];
	static bool keysDown[];
};


#endif // KEYBOARD_H