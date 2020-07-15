#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "SpectrumFronend.h"
#include "RendererEditor.h" 
#include "SpWindowEventListener.h"
#include "SpEventDispatcher.h"
#include "Timer.h"
#include "SpPhysicSimulator.h"
#include "GpuContext.h"

namespace NAMESPACE_FRONTEND
{
	class Application : 
		public Object, 
		public SpWindowEventListener
	{
	private:
		sp_bool isRunning;
		Timer timer;
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

			gpuContext = GpuContext::init();

			const sp_uint maxObjects = 1024u;
			SpPhysicSimulator::init(maxObjects);
			SpGraphicObjectManager::instance()->init(maxObjects);

			SpEventDispatcher::instance()->addWindowListener(this);

			editor->setWindow(window);
			editor->init();
		}

		API_INTERFACE void start()
		{
#if defined(WINDOWS) || defined(LINUX) || defined(MAC)
			sp_float elapsedTime = ZERO_FLOAT;
			
			timer.start();

			while (isRunning)
			{
				elapsedTime = timer.elapsedTime();

#ifdef DEBUG
				if (elapsedTime > timer.framePerSecondLimit())
					elapsedTime = timer.framePerSecondLimit() * 0.5f;
#endif


				SpEventDispatcher::instance()->processAllEvents();

				if (SpPhysicSettings::instance()->isSimulationEnabled())
				{
					//do {
						editor->update(elapsedTime);
						SpPhysicSimulator::instance()->run(timer);
					//elapsedTime = timer.elapsedTime();
					//} while (elapsedTime * 2.0f < timer.framePerSecondLimit());
				}

				//timeInterpolated = timer.getFramesPerSecond() + SKIP_TICKS - FRAMES_PER_SECOND_LIMIT / SKIP_TICKS;
				//render(timeInterpolated);

				editor->preRender();
				editor->render();
				editor->postRender();
				editor->renderGUI();

				if (!isRunning)
					break;

#ifdef DEBUG
				LogGL::glErrors(__FILE__, __LINE__);
#endif // DEBUG

				timer.update();

				window->refresh();
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
			if (gpuContext != nullptr)
			{
				sp_mem_delete(gpuContext, GpuContext);
				gpuContext = nullptr;
			}
		}

		API_INTERFACE inline const sp_char* toString()
		{
			return "SpApplication";
		}

	};
}

#endif // APPLICATION_HEADER