#ifdef UNIX
    #define GL_SILENCE_DEPRECATION
#endif

#define WINDOW_TITLE "Spectrum Engine"

#include "SpectrumFronend.h"
#include "MainFrame.h"
#include "DefaultRendererManager.h"
#include "RendererSettings.h"
#include "DisplayDeviceGLFW.h"
#include "KeyboardInputDeviceGLFW.h"
#include "PointerInputDeviceGLFW.h"
#include "WindowInputDeviceGLFW.h"

using namespace NAMESPACE_FRONTEND;

static void glfw_error_callback(sp_int error, const sp_char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

sp_int main(sp_int, sp_char**)
{
	StackMemoryAllocator::main()->init();

	DisplayDeviceGLFW* monitor = new DisplayDeviceGLFW();
	KeyboardInputDeviceGLFW* keyboard = new KeyboardInputDeviceGLFW();
	PointerInputDeviceGLFW* mouse = new PointerInputDeviceGLFW();
	WindowInputDeviceGLFW* windowsDevice = WindowInputDeviceGLFW::getInstance();

	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit())
		return 1;

#ifdef OPENGLES_ENABLED
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
#endif

	Vec2i windowPosition = { 300, 300 };
	Vec2f windowSize = { 1280, 720 };
	GLFWwindow* window = glfwCreateWindow((sp_int)windowSize.x, (sp_int)windowSize.y, "Spectrum Engine", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	GLenum glewinit = glewInit();
	if (glewinit != GLEW_OK)
	{
		std::string errorMessage = reinterpret_cast<sp_char*>(((GLubyte*)glewGetErrorString(glewinit)));
		Log::error(errorMessage);
		return -1;
	}

	glfwSetWindowTitle(window, WINDOW_TITLE);

	RendererSize::getInstance()->init();
	RendererSize::getInstance()->resize(windowSize.x, windowSize.y);
	NAMESPACE_RENDERING::RendererSettings::getInstance()->setRendererPosition(Vec2f((sp_float)windowPosition.x, (sp_float)windowPosition.y));
	NAMESPACE_RENDERING::RendererSettings::getInstance()->setSize(windowSize.x, windowSize.y);

	monitor->init(window);
	keyboard->init(window);
	mouse->init(window);
	windowsDevice->init(window);

	MainFrame engineEditor;
	engineEditor.setWindow(window);

	NAMESPACE_RENDERING::DefaultRendererManager* renderer = new NAMESPACE_RENDERING::DefaultRendererManager();
	renderer->setRendererEditor(&engineEditor);
	renderer->init(monitor);
	renderer->addInputDevice(mouse);
	renderer->addInputDevice(keyboard);
	renderer->addInputDevice(windowsDevice);

	//keyboard->addHandler(engineEditor);
	//mouse->addHandler(engineEditor);
	windowsDevice->addHandler(&engineEditor);

	renderer->resize(windowSize.x, windowSize.y);
	renderer->start();

	delete renderer;
	glfwDestroyWindow(window);
	glfwTerminate();

	StackMemoryAllocator::main()->release();
	return 0;
}