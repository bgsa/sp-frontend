#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "SpectrumFronend.h"
#include "RendererEditor.h" 
#include "SpWindowEventListener.h"
#include "SpEventDispatcher.h"
#include "Timer.h"
#include "SpPhysicSimulator.h"
#include "GpuContext.h"
#include "SpGpuRenderingFactoryOpenGL.h"
#include "SpLogger.h"
#include "SpLogConsoleProvider.h"
#include "SpLogEngineProvider.h"

namespace NAMESPACE_FRONTEND
{
	class Application : 
		public Object, 
		public SpWindowEventListener
	{
	private:
		sp_bool isRunning;
		SpUIManager* editor;
		SpWindow* window;
		GpuContext* gpuContext;

	public:

		API_INTERFACE Application()
		{
			isRunning = true;
			window = nullptr;
			editor = nullptr;
			gpuContext = nullptr;
		}

		API_INTERFACE void setEngineEditor(SpUIManager* editor)
		{
			this->editor = editor;
		}

		API_INTERFACE void init(SpWindow* window)
		{
			this->window = window;

			SpLogger::init();
			SpLogEngineProvider* logEngineProvider = sp_mem_new(SpLogEngineProvider);
			SpLogConsoleProvider* logConsoleProvider = sp_mem_new(SpLogConsoleProvider);
			SpLogger::instance()->addProvider(logEngineProvider);
			SpLogger::instance()->addProvider(logConsoleProvider);

			gpuContext = GpuContext::init();
			SpGpuRenderingFactoryOpenGL::init();

			SpWorldManager::init();

			SpEventDispatcher::instance()->addWindowListener(this);

			editor->setWindow(window);
			editor->init();

			SpWorldManagerInstance->current()->initMeshCache();
		}

		API_INTERFACE void start()
		{
#if defined(WINDOWS) || defined(LINUX) || defined(MAC)

			SpWorldManagerInstance->current()->physicSimulator->moveAwayDynamicObjects(); // remove initial collisions
			SpPhysicSettings* physicSettings = SpPhysicSettings::instance();

			Timer::init();

			while (isRunning)
			{
				Timer::frameTimer()->update();

				SpEventDispatcher::instance()->processAllEvents();

				if (SpPhysicSettings::instance()->isSimulationEnabled())
				{
					//const sp_float elapsedTime = Timer::physicTimer()->elapsedTime();
					const sp_float elapsedTime = 30.0f;

					editor->update(elapsedTime);

					SpWorldManagerInstance->current()->update(elapsedTime);

					Timer::physicTimer()->update();
				}

				SpWorldManagerInstance->current()->updateTransformsOnGPU();

				editor->preRender();
				editor->render();
				Timer::renderTimer()->update();
				editor->postRender();
				editor->renderGUI();
#ifdef DEBUG
				LogGL::glErrors(__FILE__, __LINE__);
#endif
				window->refresh();

				if (Timer::frameTimer()->elapsedTime() < Timer::frameTimer()->framePerSecondLimit())
				{
					const sp_int msToWait = (int)(Timer::frameTimer()->framePerSecondLimit() - Timer::frameTimer()->elapsedTime());
					std::this_thread::sleep_for(std::chrono::milliseconds(msToWait)); // fix to FPS limit
				}

				physicSettings->nextFrame();
			}
#endif
		}

		API_INTERFACE virtual void stop()
		{
			isRunning = false;
		}

		API_INTERFACE void onWindowEvent(SpWindowEvent* evt) override
		{
			switch (evt->type)
			{
			case (sp_uint)SpWindowEventType::Closed:
				stop();
				break;

			default:
				break;
			}
		}

		API_INTERFACE inline void dispose() override 
		{
			SpWorldManager::dispose();

			if (gpuContext != nullptr)
			{
				sp_mem_delete(gpuContext, GpuContext);
				gpuContext = nullptr;
			}

			SpLogger::dispose();
		}

		API_INTERFACE inline const sp_char* toString()
		{
			return "SpApplication";
		}

	};
}

#endif // APPLICATION_HEADER