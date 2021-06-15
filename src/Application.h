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
		}

		API_INTERFACE void setEngineEditor(SpUIManager* editor)
		{
			this->editor = editor;
		}

		API_INTERFACE void init(SpWindow* window)
		{
			this->window = window;

			initLogger();

			SpGpuRenderingFactoryOpenGL::init();

			sp_char* bvText = SpApplicationArgumentsInstace->get(2);
			if (std::strcmp(bvText, "DOP18") == 0)
			{
				SpPhysicSettings::instance()->boundingVolumeType(BoundingVolumeType::DOP18);
			}
			else if (std::strcmp(bvText, "AABB") == 0)
			{
				SpPhysicSettings::instance()->boundingVolumeType(BoundingVolumeType::AABB);
			}
			else if (std::strcmp(bvText, "SPHERE") == 0)
			{
				SpPhysicSettings::instance()->boundingVolumeType(BoundingVolumeType::Sphere);
			}

			SpPhysicSettings::instance()->enableSimulation();

			SpProjectManager::init();
			std::cout << errno << std::endl;
			SpWorldManager::init();

			SpEventDispatcher::instance()->addWindowListener(this);

			editor->window = window;
			editor->init();

			SpWorldManagerInstance->current()->initMeshCache();
		}

		API_INTERFACE void start()
		{
#if defined(WINDOWS) || defined(LINUX) || defined(MAC)

			sp_float buildBVTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&buildBVTime);

			sp_uint paresSAP = ZERO_UINT;
			SpGlobalPropertiesInscance->add(&paresSAP);

			// SAP
			sp_float buildElementsTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&buildElementsTime);
			sp_float radixSortingTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&radixSortingTime);
			sp_float sapTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&sapTime);

			// GJK
			sp_uint gjkEpaCount = ZERO_UINT;
			SpGlobalPropertiesInscance->add(&gjkEpaCount);
			sp_float gjkEpaTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&gjkEpaTime);

			// SHAPE MATCHING
			sp_uint shapeMatchingCount = ZERO_UINT;
			SpGlobalPropertiesInscance->add(&shapeMatchingCount);
			sp_float shapeMatchingTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&shapeMatchingTime);

			// PCA
			sp_float pcaTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&pcaTime);
			sp_uint qtdAlteracoesPCA = ZERO_UINT;
			SpGlobalPropertiesInscance->add(&qtdAlteracoesPCA);
			Vec3 eixoPCA = Vec3Zeros;
			SpGlobalPropertiesInscance->add(&eixoPCA);

			// RENDERING
			sp_float renderingTime = ZERO_FLOAT;
			SpGlobalPropertiesInscance->add(&renderingTime);

			// TODO: REMOVER !
			sp_size maxFrame;
			sp_char* temp = SpApplicationArgumentsInstace->get(3);
			convert(temp, maxFrame);
			sp_char* filename = SpApplicationArgumentsInstace->get(4);
			SpCSVFileWriter csvFile(filename);
			csvFile
				.addHeader("FRAME ID")

				->addHeader("TEMPO BUILD VOLUME")
				
				->addHeader("TEMPO BUILD ELEMENTS RADIX")
				->addHeader("TEMPO RADIX")
				->addHeader("TEMPO PRUNE")
				->addHeader("TEMPO SAP")

				->addHeader("PARES SAP")
				
				->addHeader("QTD GJKEPA")
				->addHeader("TEMPO GJKEPA")

				->addHeader("QTD SHAPE_MATCHING")
				->addHeader("TEMPO SHAPE_MATCHING")

				->addHeader("TEMPO PCA")
				->addHeader("QTD ALTERACOES PCA")
				->addHeader("EIXO PCA")

				->addHeader("RENDERING TIME")

				->addHeader("FRAME TIME")
				->newRecord();

			sp_char filenameGroup[512];
			strReplace(filename, ".csv", "-group.csv", filenameGroup);
			SpCSVFileWriter csvFileGroup(filenameGroup);
			csvFileGroup
				.addHeader("FRAME ID")
				->addHeader("TEMPO BUILD VOLUME")
				->addHeader("TEMPO SAP")
				->addHeader("TEMPO GJKEPA")
				->addHeader("TEMPO SHAPE_MATCHING")
				->addHeader("TEMPO PCA")
				->addHeader("RENDERING TIME")
				->addHeader("FRAME TIME")
				->newRecord();
			sp_float accBuildVolume, accSap, accGJKEPA, accShapeMatching, accPCA, accRendering, accFrameTime;
			accBuildVolume = accSap = accGJKEPA = accShapeMatching = accPCA = accRendering = accFrameTime = ZERO_FLOAT;

			Timer::init();

			while (isRunning)
			{
				sp_int* checkPoint = ALLOC_ARRAY(sp_int, ONE_UINT);
				Timer::frameTimer()->update();

				SpEventDispatcher::instance()->processAllEvents();

				if (SpPhysicSettings::instance()->isSimulationEnabled())
				{
					// TODO: REMOVE
					((sp_float*)SpGlobalPropertiesInscance->get(ID_radixSortingTime))[0] = ZERO_FLOAT;
					((sp_float*)SpGlobalPropertiesInscance->get(ID_pcaTime))[0] = ZERO_FLOAT;
					((sp_uint*)SpGlobalPropertiesInscance->get(ID_gjkEpaCount))[0] = ZERO_UINT;
					((sp_float*)SpGlobalPropertiesInscance->get(ID_gjkEpaTime))[0] = ZERO_FLOAT;
					((sp_uint*)SpGlobalPropertiesInscance->get(ID_shapeMatchingCount))[0] = ZERO_UINT;
					((sp_float*)SpGlobalPropertiesInscance->get(ID_shapeMatchingTime))[0] = ZERO_FLOAT;
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
				sp_size frameId = SpPhysicSettings::instance()->frameId();
				csvFile.addValue(frameId);

				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_buildVolumeTime)));

				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_buildElementsTime)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_radixSortingTime)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_pruneTime)));

				sp_float sapTime
					= *((sp_float*)SpGlobalPropertiesInscance->get(ID_buildElementsTime))
					+ *((sp_float*)SpGlobalPropertiesInscance->get(ID_radixSortingTime))
					+ *((sp_float*)SpGlobalPropertiesInscance->get(ID_pruneTime));
				csvFile.addValue(sapTime);

				csvFile.addValue(*((sp_uint*)SpGlobalPropertiesInscance->get(ID_paresSAP)));
				
				csvFile.addValue(*((sp_uint*)SpGlobalPropertiesInscance->get(ID_gjkEpaCount)));
				csvFile.addValue(*((sp_float*)SpGlobalPropertiesInscance->get(ID_gjkEpaTime)));

				csvFile.addValue(*((sp_uint*)SpGlobalPropertiesInscance->get(ID_shapeMatchingCount)));
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



				accBuildVolume += *((sp_float*)SpGlobalPropertiesInscance->get(ID_buildVolumeTime));
				accSap += sapTime;
				accGJKEPA += *((sp_float*)SpGlobalPropertiesInscance->get(ID_gjkEpaTime));
				accShapeMatching += *((sp_float*)SpGlobalPropertiesInscance->get(ID_shapeMatchingTime));
				accPCA += *((sp_float*)SpGlobalPropertiesInscance->get(ID_pcaTime));
				accRendering += *((sp_float*)SpGlobalPropertiesInscance->get(ID_renderingTime));
				accFrameTime += frameTime;

				if ((frameId + 1) % 10 == 0)
				{
					csvFileGroup.addValue(frameId + 1);
					csvFileGroup.addValue(accBuildVolume / 10.0f);
					csvFileGroup.addValue(accSap / 10.0f);
					csvFileGroup.addValue(accGJKEPA / 10.0f);
					csvFileGroup.addValue(accShapeMatching / 10.0f);
					csvFileGroup.addValue(accPCA / 10.0f);
					csvFileGroup.addValue(accRendering / 10.0f);
					csvFileGroup.addValue(accFrameTime / 10.0f);
					csvFileGroup.newRecord();
					accBuildVolume = accSap = accGJKEPA = accShapeMatching = accPCA = accRendering = accFrameTime = ZERO_FLOAT;
				}

				if (frameId == maxFrame)
					isRunning = false;

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
			SpLogger::dispose();
		}

		API_INTERFACE inline const sp_char* toString()
		{
			return "SpApplication";
		}

	};
}

#endif // APPLICATION_HEADER