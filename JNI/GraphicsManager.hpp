#ifndef _PACKT_GRAPHICSMANAGER_HPP_
#define _PACKT_GRAPHICSMANAGER_HPP_

#include "Resource.hpp"
#include "../Types.hpp"

static const char* VERTEX_SHADER =
"attribute vec2 aPosition;\n"
"attribute vec2 aTexture;\n"
"varying vec2 vTexture;\n"
"void main() {\n"
" vTexture = aTexture;\n"
" gl_Position = vec4(aPosition, 1.0, 1.0 );\n"
"}";

static const char* FRAGMENT_SHADER =
"precision mediump float;"
"uniform sampler2D uTexture;\n"
"varying vec2 vTexture;\n"
"void main() {\n"
" gl_FragColor = texture2D(uTexture, vTexture);\n"
"}\n";

struct TextureProperties 
{
	Resource* textureResource;
	GLuint texture;
	int32_t width;
	int32_t height;
};

class GraphicsComponent {
public:
	virtual status load() = 0;
	virtual void draw() = 0;
};

class GraphicsManager {
public:
	GraphicsManager();
	~GraphicsManager();
	int32_t getRenderWidth() { return mRenderWidth; }
	int32_t getRenderHeight() { return mRenderHeight; }
	int32_t getScreenWidth() { return mScreenWidth; }
	int32_t getScreenHeight() { return mScreenHeight; }
	GLfloat* getProjectionMatrix() { return mProjectionMatrix[0]; }
	void registerComponent(GraphicsComponent* pComponent);
	status start();
	void stop();
	status update();
	TextureProperties* loadTexture(Resource& pResource);
	GLuint loadShader(const char* pVertexShader, const char* pFragmentShader);
	GLuint loadVertexBuffer(const void* pVertexBuffer, int32_t pVertexBufferSize);
private:
	status initializeRenderBuffer();
	struct RenderVertex {
		GLfloat x, y, u, v;
	};
	int32_t mRenderWidth; int32_t mRenderHeight;
	int32_t mScreenWidth; int32_t mScreenHeight;
	GLfloat mProjectionMatrix[4][4];
	TextureProperties mTextures[32]; int32_t mTextureCount;
	GLuint mShaders[32]; int32_t mShaderCount;
	GLuint mVertexBuffers[32]; int32_t mVertexBufferCount;
	GraphicsComponent* mComponents[32]; int32_t mComponentCount;
	// Rendering resources.
	GLint mScreenFrameBuffer;
	GLuint mRenderFrameBuffer; GLuint mRenderVertexBuffer;
	GLuint mRenderTexture; GLuint mRenderShaderProgram;
	GLuint aPosition; GLuint aTexture;
	GLuint uProjection; GLuint uTexture;

	// glfw resources
	GLFWwindow* mpWindow;
	GLFWmonitor* mpMonitor;
};
#endif