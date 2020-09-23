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
		SpPhysicSimulator* physicSimulator;

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

			SpLogEngineProvider* logProvider = sp_mem_new(SpLogEngineProvider);
			SpLogger::init(logProvider);

			gpuContext = GpuContext::init();
			SpGpuRenderingFactoryOpenGL::init();

			//const sp_uint maxObjects = 2u;
			const sp_uint maxObjects = 1024u;
			physicSimulator = SpPhysicSimulator::init(maxObjects);

			SpEventDispatcher::instance()->addWindowListener(this);

			editor->setWindow(window);
			editor->init();
		}

		API_INTERFACE void start()
		{
#if defined(WINDOWS) || defined(LINUX) || defined(MAC)

			SpPhysicSimulator::instance()->moveAwayDynamicObjects(); // remove initial collisions

			Timer::init();

			while (isRunning)
			{
				Timer::frameTimer()->update();

				SpEventDispatcher::instance()->processAllEvents();

				if (SpPhysicSettings::instance()->isSimulationEnabled())
				{
					physicSimulator->run(); // update collisions and responses
					//editor->update(Timer::physicTimer()->elapsedTime()); // integrate all objects
					
					// TODO: REMOVER
					editor->update(30.0f); // fixed to 30 

					Timer::physicTimer()->update();
				}

				physicSimulator->updateTransformsOnGPU(); // update data on GPU

				editor->preRender();
				editor->render();
				Timer::renderTimer()->update();
				editor->postRender();
				editor->renderGUI();

#ifdef DEBUG
				LogGL::glErrors(__FILE__, __LINE__);
#endif // DEBUG

				window->refresh();

				if (Timer::frameTimer()->elapsedTime() < Timer::frameTimer()->framePerSecondLimit())
				{
					const sp_int msToWait = (int)(Timer::frameTimer()->framePerSecondLimit() - Timer::frameTimer()->elapsedTime());
					std::this_thread::sleep_for(std::chrono::milliseconds(msToWait)); // fix to FPS limit
				}
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
			if (physicSimulator != nullptr)
			{
				sp_mem_delete(physicSimulator, SpPhysicSimulator);
				physicSimulator = nullptr;
			}

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