#define WINDOW_TITLE "Spectrum Engine"

#include "SpectrumFronend.h"
#include "SpUIManager.h"
#include "SpWindowGLFW.h"
#include "Application.h"
#include "SpThreadPool.h"

using namespace NAMESPACE_FRONTEND;

static void initGlew()
{
	GLenum glewinit = glewInit();
	if (glewinit != GLEW_OK)
	{
		const sp_char* errorMessage = reinterpret_cast<sp_char*>(((GLubyte*)glewGetErrorString(glewinit)));
		Log::error(errorMessage);
	}
}

sp_int main(sp_int, sp_char**)
{
	StackMemoryAllocator::main()->init();
	SpThreadPool::instance()->init();

	SpWindowGLFW window;
	window.init();
	window.setTitle(WINDOW_TITLE);

	initGlew();

	SpUIManager engineEditor;
	engineEditor.setWindow(&window);

	Application app;
	app.setEngineEditor(&engineEditor);
	app.init(&window);
	app.start();

	engineEditor.dispose();
	app.dispose();
	window.dispose();

	StackMemoryAllocator::main()->release();
	return 0;
}