#define WINDOW_TITLE "Spectrum Engine"

#include "SpectrumFronend.h"
#include "SpUIManager.h"
#include "SpWindowGLFW.h"
#include "Application.h"
#include "SpThreadPool.h"
#include "SpGlobalProperties.h"
#include "SpApplicationArguments.h"

using namespace NAMESPACE_FRONTEND;

sp_int main(sp_int parametersLength, sp_char** parameters)
{
	SpStackMemoryAllocator::main()->init(SP_STACK_MEMORY_DEFAULT_SIZE);
	//	SpThreadPool::instance()->init();

	SpApplicationArguments::init(parametersLength, parameters);
	SpGlobalProperties::init();
	SpPhysicSettings::instance()->enableProfiling();

	SpWindowGLFW window;
	window.init();
	window.setTitle(WINDOW_TITLE);

	NAMESPACE_RENDERING::SpOpenGL::init();

	SpUIManager engineEditor;
	engineEditor.setWindow(&window);

	Application app;
	app.setEngineEditor(&engineEditor);
	app.init(&window);
	app.start();

	engineEditor.dispose();
	app.dispose();
	window.dispose();

	SpGlobalProperties::dispose();
	SpApplicationArguments::dispose();

	SpStackMemoryAllocator::main()->dispose();
	return 0;
}