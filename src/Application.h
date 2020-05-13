#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "SpectrumFronend.h"
#include "SpEventDispatcher.h"
#include "IRendererManager.h"
#include "RendererEditor.h" 
#include "Timer.h"

namespace NAMESPACE_FRONTEND
{
	class Application
		: public Object
	{
	private:
		sp_bool isRunning;
		Timer timer;
		SpUIManager* editor;
		SpWindow* window;

		void update()
		{
			timer.update();
			sp_longlong elapsedTime = timer.getElapsedTime();

			editor->update(elapsedTime);
		}

	public:

		API_INTERFACE Application()
		{
			isRunning = true;
		}

		API_INTERFACE void setEngineEditor(SpUIManager* editor)
		{
			this->editor = editor;
		}

		API_INTERFACE void init(SpWindow* window)
		{
			this->window = window;
			editor->setWindow(window);
			editor->init();
		}

		API_INTERFACE void start()
		{
#if defined(WINDOWS) || defined(LINUX) || defined(MAC)
			sp_float nextTick = ZERO_FLOAT;
			sp_bool fpsLowerThanFrameLimit = false;
			sp_float elapsedTime = ZERO_FLOAT;

			timer.start();

			while (isRunning)
			{
				elapsedTime = sp_float(timer.getElapsedTime());
				nextTick = timer.getSkipTick();

				do
				{
					SpEventDispatcher::instance()->processAllEvents();
					update();

					fpsLowerThanFrameLimit = elapsedTime > nextTick;
					nextTick += timer.getSkipTick();
				} while (fpsLowerThanFrameLimit);

				//timeInterpolated = timer.getFramesPerSecond() + SKIP_TICKS - FRAMES_PER_SECOND_LIMIT / SKIP_TICKS;
				//render(timeInterpolated);

				editor->preRender();
				editor->render();
				editor->postRender();
				editor->renderGUI();

				//cout << "FPS: " << timer.getFramesPerSecond() << END_OF_LINE;
				//cout << "Elapsed Time: " << timer.getElapsedTime() << END_OF_LINE;

				timer.update();

				if (!isRunning)
					break;
				else
					LogGL::glErrors(__FILE__, __LINE__);

				window->refresh();
			}
#endif
		}

		API_INTERFACE virtual void stop()
		{
			isRunning = false;
		}

		API_INTERFACE inline void dispose() override 
		{
			//renderer->dispose();
			//sp_mem_delete(renderer, DefaultRendererManager);
		}

		API_INTERFACE inline const sp_char* toString()
		{
			return "SpApplication";
		}

	};
}

#endif // APPLICATION_HEADER