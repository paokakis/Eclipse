#include "../Types.hpp"
#include "GraphicsManager.hpp"
#include "../Log.hpp"
#include "SOIL.h"
#include "Keyboard.hpp"

#define WIDTH	(800)
#define HEIGHT	(600)



const int32_t DEFAULT_RENDER_WIDTH = 600;
bool quit = false;

static void window_close_callback(GLFWwindow* window)
{
	if (!quit)
		glfwSetWindowShouldClose(window, GLFW_FALSE);
}

GraphicsManager::GraphicsManager() :
	mRenderWidth(0), mRenderHeight(0),
	mProjectionMatrix(),
	mTextures(), mTextureCount(0),
	mShaders(), mShaderCount(0),
	mVertexBuffers(), mVertexBufferCount(0),
	mComponents(), mComponentCount(0),
	mScreenFrameBuffer(0),
	mRenderFrameBuffer(0), mRenderVertexBuffer(0),
	mRenderTexture(0), mRenderShaderProgram(0),
	aPosition(0), aTexture(0),
	uProjection(0), uTexture(0),
	mScreenWidth(WIDTH), mScreenHeight(HEIGHT)
{
	Log::info("Creating GraphicsManager.");
}

GraphicsManager::~GraphicsManager() 
{
	Log::info("Destroying GraphicsManager.");
}

void GraphicsManager::registerComponent(GraphicsComponent* pComponent)
{
	mComponents[mComponentCount++] = pComponent;
}

status GraphicsManager::start() 
{
	Log::info("Starting GraphicsManager.");

	// GLFW init
	if (!glfwInit()) {
		Log::error("Error initialising GLFW");

		return STATUS_KO;
	}

	mpWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, GAME_NAME, NULL, NULL);

	if (!mpWindow) {
		Log::error("Error creating window");

		return STATUS_KO;
	}

	// GLFW setup
	glfwMakeContextCurrent(mpWindow);
	glfwGetFramebufferSize(mpWindow, &mScreenWidth, &mScreenHeight);
	glfwSwapInterval(1); // swap back buffer and front buffer every frame

	//glfwSetCursorPosCallback(window, Mouse::mousePosCallback);  // set the callback function for handling mouse position
	//glfwSetMouseButtonCallback(window, Mouse::mouseButtonsCallback); // set the callback function to handle mouse button events
	glfwSetKeyCallback(mpWindow, Keyboard::keyCallback);

	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int xPos = (vidMode->width - mScreenWidth) / 2;
	int yPos = (vidMode->height - mScreenHeight) / 2;
	glfwSetWindowPos(mpWindow, xPos, yPos);

	// GL setup
	// viewport
	glViewport(0, 0, mScreenWidth, mScreenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, mScreenWidth, 0, mScreenHeight, -10, 10);
	glDepthRange(-10, 10);
	glMatrixMode(GL_MODELVIEW);

	// Alpha blending
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

	const GLubyte* ver_str = glGetString(GL_VERSION);
	Log::info("Opengl version %s", ver_str);

	GLenum err = glewInit();
	glViewport(0, 0, mRenderWidth, mRenderHeight);
	glDisable(GL_DEPTH_TEST);

	// Prepares the projection matrix with viewport dimesions.
	memset(mProjectionMatrix[0], 0, sizeof(mProjectionMatrix));
	mProjectionMatrix[0][0] = 2.0f / GLfloat(mRenderWidth);
	mProjectionMatrix[1][1] = 2.0f / GLfloat(mRenderHeight);
	mProjectionMatrix[2][2] = -1.0f;
	mProjectionMatrix[3][0] = -1.0f;
	mProjectionMatrix[3][1] = -1.0f;
	mProjectionMatrix[3][2] = 0.0f;
	mProjectionMatrix[3][3] = 1.0f;

	// Loads graphics components.
	for (int32_t i = 0; i < mComponentCount; ++i)
	{
		if (mComponents[i]->load() != STATUS_OK)
		{
			return STATUS_KO;
		}
	}

	return STATUS_OK;
_ERROR_:
	Log::error("Error while starting GraphicsManager");
	stop();
	return STATUS_KO;	
}

status GraphicsManager::update() 
{
	static float clearColor = 0.0f;

	glBindFramebuffer(GL_FRAMEBUFFER, mRenderFrameBuffer);
	glViewport(0, 0, mRenderWidth, mRenderHeight);

	glClearColor(clearColor, clearColor, clearColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int32_t i = 0; i < mComponentCount; ++i) 
	{
		mComponents[i]->draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, mScreenFrameBuffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, mScreenWidth, mScreenHeight);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mRenderTexture);
	glUseProgram(mRenderShaderProgram);
	glUniform1i(uTexture, 0);
	// Indicates to OpenGL how position and uv coordinates are stored.
	glBindBuffer(GL_ARRAY_BUFFER, mRenderVertexBuffer);
	glEnableVertexAttribArray(aPosition);
	glVertexAttribPointer(aPosition, // Attribute Index
		2, // Number of components (x and y)
		GL_FLOAT, // Data type
		GL_FALSE, // Normalized
		sizeof(RenderVertex), // Stride
		(GLvoid*)0); // Offset
	glEnableVertexAttribArray(aTexture);
	glVertexAttribPointer(aTexture, // Attribute Index
		2, // Number of components (u and v)
		GL_FLOAT, // Data type
		GL_FALSE, // Normalized
		sizeof(RenderVertex), // Stride
		(GLvoid*)(sizeof(GLfloat) * 2)); // Offset
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Shows the result to the user.
	glfwSwapBuffers(mpWindow);
	
	return STATUS_OK;
}

void GraphicsManager::stop() 
{
	Log::info("Stopping GraphicsManager.");

	for (int32_t i = 0; i < mTextureCount; ++i) 
	{
		glDeleteTextures(1, &mTextures[i].texture);
	}
	mTextureCount = 0;

	for (int32_t i = 0; i < mShaderCount; ++i) 
	{
		glDeleteProgram(mShaders[i]);
	}
	mShaderCount = 0;

	for (int32_t i = 0; i < mVertexBufferCount; ++i) 
	{
		glDeleteBuffers(1, &mVertexBuffers[i]);
	}
	mVertexBufferCount = 0;

	if (mRenderFrameBuffer != 0) 
	{
		glDeleteFramebuffers(1, &mRenderFrameBuffer);
		mRenderFrameBuffer = 0;
	}
	if (mRenderTexture != 0) 
	{
		glDeleteTextures(1, &mRenderTexture);
		mRenderTexture = 0;
	}

	// Destroys window.  OpenGL context.
	if (glfwWindowShouldClose(mpWindow)) 
	{
		quit = true;
	}
}

TextureProperties* GraphicsManager::loadTexture(Resource& pResource) 
{
	Log::info("Loading texture %s", pResource.getPath());
	
	GLuint texture = SOIL_load_OGL_texture(pResource.getPath(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y);
	int32_t width, height, mipLevel = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, mipLevel, GL_TEXTURE_HEIGHT, &height);
	Log::info("Texture size: %d x %d texture id %u", width, height, texture);

	if (texture == 0)
	{
		goto _ERROR_;
	}

	TextureProperties* textureProperties;
	// Caches the loaded texture.
	textureProperties = &mTextures[mTextureCount++];
	textureProperties->texture = texture;
	textureProperties->textureResource = &pResource;
	textureProperties->width = width;
	textureProperties->height = height;

	return textureProperties;

_ERROR_:
	Log::error("Error loading texture into OpenGL.");

	return NULL;
}

GLuint GraphicsManager::loadShader(const char* pVertexShader, const char* pFragmentShader) 
{
	GLint result; char log[256];
	GLuint vertexShader, fragmentShader, shaderProgram;
	// Builds the vertex shader.
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &pVertexShader, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) 
	{
		glGetShaderInfoLog(vertexShader, sizeof(log), 0, log);
		Log::error("Vertex shader error: %s", log);
		goto _ERROR_;
	}
	// Builds the fragment shader.
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &pFragmentShader, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) 
	{
		glGetShaderInfoLog(fragmentShader, sizeof(log), 0, log);
		Log::error("Fragment shader error: %s", log);
		goto _ERROR_;
	}
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (result == GL_FALSE) 
	{
		glGetProgramInfoLog(shaderProgram, sizeof(log), 0, log);
		Log::error("Shader program error: %s", log);
		goto _ERROR_;
	}
	mShaders[mShaderCount++] = shaderProgram;
	return shaderProgram;
_ERROR_:
	Log::error("Error loading shader.");
	if (vertexShader > 0) glDeleteShader(vertexShader);
	if (fragmentShader > 0) glDeleteShader(fragmentShader);
	return 0;
}

GLuint GraphicsManager::loadVertexBuffer(const void* pVertexBuffer, int32_t pVertexBufferSize) 
{
	GLuint vertexBuffer;
	// Upload specified memory buffer into OpenGL.
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, pVertexBufferSize, pVertexBuffer, GL_STATIC_DRAW);
	// Unbinds the buffer.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (glGetError() != GL_NO_ERROR) 
		goto _ERROR_;
	mVertexBuffers[mVertexBufferCount++] = vertexBuffer;

	return vertexBuffer;

_ERROR_:
	Log::error("Error loading vertex buffer.");
	if (vertexBuffer > 0) glDeleteBuffers(1, &vertexBuffer);

	return 0;
}

/*In initializeRenderBuffer(), create a predefined array of a vertex that is
going to be loaded into OpenGL. It represents a single quad with a full texture
rendered on it.
Compute the new render height based on a fixed target width of 600 pixels.
Retrieve the current screen framebuffer from the location where the
final scene is rendered using glGetIntegerv() and the special value
GL_FRAMEBUFFER_BINDING:*/
status GraphicsManager::initializeRenderBuffer() {
	Log::info("Loading offscreen buffer");
	const RenderVertex vertices[] = {
		{ -1.0f, -1.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f }
	};
	float screenRatio = float(mScreenHeight) / float(mScreenWidth);
	mRenderWidth = DEFAULT_RENDER_WIDTH;
	mRenderHeight = float(mRenderWidth) * screenRatio;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mScreenFrameBuffer);
	/*Create a texture for off-screen rendering, like we have seen previously. In
	glTexImage2D(), pass a NULL value as the last parameter to create only the
	surface without initializing its content:*/
	glGenTextures(1, &mRenderTexture);
	glBindTexture(GL_TEXTURE_2D, mRenderTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mRenderWidth, mRenderHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
	/*Then, create an off-screen framebuffer with glGenFramebuffers().
	Attach the previous texture to it with glBindFramebuffer().
	Terminate by restoring the device state:*/
	glGenFramebuffers(1, &mRenderFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	/*Create the shader program used to render texture to screen and retrieve its
	attributes and uniforms:*/
	mRenderVertexBuffer = loadVertexBuffer(vertices, sizeof(vertices));
	if (mRenderVertexBuffer == 0) 
		goto _ERROR_;
	mRenderShaderProgram = loadShader(VERTEX_SHADER, FRAGMENT_SHADER);
	if (mRenderShaderProgram == 0) 
		goto _ERROR_;
	aPosition = glGetAttribLocation(mRenderShaderProgram, "aPosition");
	aTexture = glGetAttribLocation(mRenderShaderProgram, "aTexture");
	uTexture = glGetUniformLocation(mRenderShaderProgram, "uTexture");

	return STATUS_OK;

_ERROR_:
	Log::error("Error while loading offscreen buffer");

	return STATUS_KO;
}