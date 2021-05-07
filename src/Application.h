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
#include "SpProjectManager.h"
#include "SpGlobalProperties.h"
#include "SpCSVFileWriter.h"

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

		void initLogger()
		{
			SpLogger::init();

			//SpLogEngineProvider* logEngineProvider = sp_mem_new(SpLogEngineProvider);
			SpLogConsoleProvider* logConsoleProvider = sp_mem_new(SpLogConsoleProvider);

			//SpLogger::instance()->addProvider(logEngineProvider);
			SpLogger::instance()->addProvider(logConsoleProvider);
		}

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

			initLogger();

			gpuContext = GpuContext::init();
			SpGpuRenderingFactoryOpenGL::init();

			SpProjectManager::init();
			SpWorldManager::init();

			SpEventDispatcher::instance()->addWindowListener(this);

			editor->setWindow(window);
			editor->init();

			SpWorldManagerInstance->current()->initMeshCache();
		}

		API_INTERFACE void start()
		{
#if defined(WINDOWS) || defined(LINUX) || defined(MAC)

			Timer::init();

			sp_float buildDOP18Time = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&buildDOP18Time);
			sp_uint paresDOP18 = ZERO_UINT;
			SpGlobalPropertiesInscance->add(&paresDOP18);
			sp_float sapDOP18Time = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&sapDOP18Time);
			sp_float buildElementsDOP18Time = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&buildElementsDOP18Time);

			sp_float buildAABBTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&buildAABBTime);
			sp_uint paresAABB = ZERO_UINT;
			SpGlobalPropertiesInscance->add(&paresAABB);
			sp_float sapAABBTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&sapAABBTime);
			sp_float buildElementsAABBTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&buildElementsAABBTime);

			sp_float buildSphereTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&buildSphereTime);
			sp_uint paresSphere = ZERO_UINT;
			SpGlobalPropertiesInscance->add(&paresSphere);
			sp_float sapSphereTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&sapSphereTime);
			sp_float buildElementsSphereTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&buildElementsSphereTime);

			sp_float radixSortingTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&radixSortingTime);

			sp_float gjkEpaTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&gjkEpaTime);

			sp_float shapeMatchingTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&shapeMatchingTime);

			sp_float pcaTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&pcaTime);
			sp_uint qtdAlteracoesPCA = ZERO_UINT;
			SpGlobalPropertiesInscance->add(&qtdAlteracoesPCA);
			Vec3 eixoPCA = Vec3Zeros;
			SpGlobalPropertiesInscance->add(&eixoPCA);

			sp_float renderingTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&renderingTime);

			// TODO: REMOVER !
			SpCSVFileWriter csvFile("resultado.csv");
			csvFile
				.addHeader("FRAME ID")

				->addHeader("TEMPO BUILD DOP18")
				->addHeader("PARES DOP18")
				->addHeader("TEMPO SAP DOP18")
				->addHeader("TEMPO BUILD ELEMENTS RADIX DOP18")

				->addHeader("TEMPO BUILD AABB")
				->addHeader("PARES AABB")
				->addHeader("TEMPO SAP AABB")
				->addHeader("TEMPO BUILD ELEMENTS RADIX AABB")

				->addHeader("TEMPO BUILD SPHERE")
				->addHeader("PARES SPHERE")
				->addHeader("TEMPO SAP SPHERE")
				->addHeader("TEMPO BUILD ELEMENTS RADIX SPHERE")

				->addHeader("TEMPO RADIX")
				->addHeader("TEMPO GJKEPA")
				->addHeader("TEMPO SHAPE_MATCHING")

				->addHeader("TEMPO PCA")
				->addHeader("QTD ALTERACOES PCA")
				->addHeader("EIXO PCA")

				->addHeader("RENDERING TIME")

				->addHeader("FRAME TIME")
				->newRecord();

			while (isRunning)
			{
				sp_int* checkPoint = ALLOC_ARRAY(sp_int, ONE_UINT);
				Timer::frameTimer()->update();

				SpEventDispatcher::instance()->processAllEvents();

				if (SpPhysicSettings::instance()->isSimulationEnabled())
				{
					// TODO: REMOVE
					((sp_float*)SpGlobalPropertiesInscance->get(ID_radixSortingTime))[0] = ZERO_FLOAT;
					((sp_float*)SpGlobalPropertiesInscance->get(ID_gjkEpaTime))[0] = ZERO_FLOAT;
					((sp_float*)SpGlobalPropertiesInscance->get(ID_shapeMatchingTime))[0] = ZERO_FLOAT;
					((sp_float*)SpGlobalPropertiesInscance->get(ID_pcaTime))[0] = ZERO_FLOAT;
					((Vec3*)SpGlobalPropertiesInscance->get(ID_eixoPCA))[0] = Vec3Zeros;

					//const sp_float elapsedTime = Timer::physicTimer()->elapsedTime();
					const sp_float elapsedTime = 30.0f;

					editor->update(elapsedTime);

					if (SpWorldManager::isInitialized())
						SpWorldManagerInstance->current()->update(elapsedTime);

					Timer::physicTimer()->update();
				}

				if (SpWorldManager::isInitialized())
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

				const sp_float frameTime = Timer::frameTimer()->elapsedTime();
				if (frameTime < Timer::frameTimer()->framePerSecondLimit())
				{
					const sp_int msToWait = (int)(Timer::frameTimer()->framePerSecondLimit() - frameTime);
					std::this_thread::sleep_for(std::chrono::milliseconds(msToWait)); // fix to FPS limit
				}

				// FRAME ID
				csvFile.addValue(SpPhysicSettings::instance()->frameId());

				// DOP18
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_buildDOP18Time)));
				csvFile.addValue(*((sp_uint*)SpGlobalPropertiesInscance->get(ID_paresDOP18)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_sapDOP18Time)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_buildElementsDOP18Time)));

				// AABB
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_buildAABBTime)));
				csvFile.addValue(*((sp_uint*)SpGlobalPropertiesInscance->get(ID_paresAABB)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_sapAABBTime)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_buildElementsAABBTime)));

				// ESFERAS
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_buildSphereTime)));
				csvFile.addValue(*((sp_uint*)SpGlobalPropertiesInscance->get(ID_paresSphere)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_sapSphereTime)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_buildElementsSphereTime)));

				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_radixSortingTime)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_gjkEpaTime)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_shapeMatchingTime)));

				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_pcaTime)));
				csvFile.addValue(*((sp_uint*)SpGlobalPropertiesInscance->get(ID_qtdAlteracoesPCA)));
				const Vec3 axis = *((Vec3*)SpGlobalPropertiesInscance->get(ID_eixoPCA));
				sp_char axisAsString[100];
				SpString::convert(axis.x, axisAsString);
				std::strcat(axisAsString, "  ");
				sp_char value[30];
				SpString::convert(axis.y, value);
				std::strcat(axisAsString, value);
				std::strcat(axisAsString, "  ");
				SpString::convert(axis.z, value);
				std::strcat(axisAsString, value);
				csvFile.addValue(axisAsString);

				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_renderingTime)));
				csvFile.addValue(frameTime);
				csvFile.newRecord();

				SpPhysicSettings::instance()->nextFrame();
				ALLOC_RELEASE(checkPoint);
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
			SpProjectManager::dispose();

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