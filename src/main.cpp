#define WINDOW_TITLE "Spectrum Engine"

#include "SpectrumFronend.h"
#include "SpUIManager.h"
#include "SpWindowGLFW.h"
#include "Application.h"
#include "SpThreadPool.h"
#include "SpGlobalProperties.h"
#include "SpApplicationArguments.h"
#include "SpVariablesEnvironment.h"
#include "SpMemoryProfiling.h"
#include "SpGameEngineSettings.h"

using namespace NAMESPACE_FRONTEND;

void dispose()
{
	GpuContext::release();

	SpGlobalProperties::dispose();
	SpApplicationArguments::dispose();
	SpGameEngineSettings::release();

	SpStackMemoryAllocator::main()->dispose();
	SpMemoryProfiling::release();
}

sp_int main(sp_int parametersLength, sp_char** parameters)
{
#ifdef MEMORY_PROFILING_ENABLED
	SpMemoryProfiling::init();
#endif

	SpStackMemoryAllocator::main()->init(SP_STACK_MEMORY_DEFAULT_SIZE);
	//	SpThreadPool::instance()->init();

	setVariableEnvirontment("CUDA_CACHE_DISABLE", "1"); // disable cuda cache of kernels built

	SpGameEngineSettings::init();
	SpApplicationArguments::init(parametersLength, parameters);
	SpGlobalProperties::init();
	SpPhysicSettings::instance()->enableProfiling();
	
	SpWindowGLFW window;
	window.init();
	window.setTitle(WINDOW_TITLE);

	NAMESPACE_RENDERING::SpOpenGL::init();

	GpuContext::init();

	SpUIManager::initialize();
	SpUIManagerInstance->window = &window;

	Application app;
	app.setEngineEditor(SpUIManagerInstance);
	app.init(&window);
	app.start();

	SpUIManager::release();

	app.dispose();
	window.dispose();

	dispose();

	return 0;
}