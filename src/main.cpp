#ifdef UNIX
    #define GL_SILENCE_DEPRECATION
#endif

#define WINDOW_TITLE "Spectrum Engine"

#include "SpectrumFronend.h"
#include "SpUIManager.h"
#include "DefaultRendererManager.h"
#include "RendererSettings.h"
#include "RendererSize.h"
#include "SpWindowGLFW.h"

using namespace NAMESPACE_FRONTEND;

static void initGlew()
{
	GLenum glewinit = glewInit();
	if (glewinit != GLEW_OK)
	{
		std::string errorMessage = reinterpret_cast<sp_char*>(((GLubyte*)glewGetErrorString(glewinit)));
		Log::error(errorMessage);
	}
}

sp_int main(sp_int, sp_char**)
{
	StackMemoryAllocator::main()->init();

	SpWindowGLFW window;
	window.init();
	window.setTitle(WINDOW_TITLE);

	initGlew();

	RendererSize::getInstance()->init();
	RendererSize::getInstance()->resize((sp_float)window.state()->width, (sp_float)window.state()->height);
	RendererSettings::getInstance()->setRendererPosition(Vec2f((sp_float)window.state()->x, (sp_float)window.state()->y));
	RendererSettings::getInstance()->setSize((sp_float)window.state()->width, (sp_float)window.state()->height);

	SpUIManager engineEditor;
	engineEditor.setWindow(&window);

	DefaultRendererManager* renderer = sp_mem_new(DefaultRendererManager)();
	renderer->setRendererEditor(&engineEditor);
	renderer->init(&window);
	renderer->resize((sp_float)window.state()->width, (sp_float)window.state()->height);
	renderer->start();

	engineEditor.dispose();
	sp_mem_delete(renderer, DefaultRendererManager);
	window.dispose();
	StackMemoryAllocator::main()->release();
	return 0;
}