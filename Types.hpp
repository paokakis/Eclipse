#ifndef TYPES_H
#define TYPES_H
#include <cstdint>
#include <windows.h>
#include <shellapi.h>
#include <memory.h>
#include <math.h>
#include "../pc/Glew/include/GL/glew.h"
#include "../pc/Glfw/include/GLFW/glfw3.h"
#include <gl/gl.h>
#include "../pc/soil/src/SOIL.h"
#include "sources\Box2D\Box2D.h"
#include "Log.hpp"

#define GAME_NAME	"Space Wars"

#define STATUS_OK		(0)
#define STATUS_KO		(-1)
#define STATUS_EXIT		(-2)

typedef int32_t status;

struct sLocation {
	sLocation() : x(0.0f), y(0.0f) {};
	float x; float y;
};
#define RAND(pMax) (float(pMax) * float(rand()) / float(RAND_MAX))

#if defined(_WIN32)
typedef int int32_t;
typedef unsigned short uint16;

#elif defined(__ANDROID__)


#endif // _WIN32


#endif // TYPES_H

