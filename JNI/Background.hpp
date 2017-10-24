#ifndef _PACKT_BACKGROUND_HPP_
#define _PACKT_BACKGROUND_HPP_

#include "GraphicsManager.hpp"
#include "TimeManager.hpp"
#include "Types.hpp"
#include <GLES2/gl2.h>

class Background : public GraphicsComponent 
{
public:
	Background(android_app* pApplication, TimeManager& pTimeManager,
		GraphicsManager& pGraphicsManager, Resource& pTextureResource);
	status load();
	void draw();
private:
	struct Vertex {
		GLfloat x, y, z;
	};
	TimeManager& mTimeManager;
	GraphicsManager& mGraphicsManager;
	Resource& mTextureResource;
	GLuint mVertexBuffer; GLuint mTexture; GLuint mShaderProgram;
	GLuint aPosition; GLuint uProjection;
	GLuint uTime; GLuint uHeight; GLuint uTexture;
};

#endif