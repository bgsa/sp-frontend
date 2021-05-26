#include "GameFrame.h"

namespace NAMESPACE_FRONTEND
{
	void GameFrame::init(SpWindow* window)
	{
		SpFrame::init(window);

		framebuffer = sp_mem_new(OpenGLFramebuffer)();
		framebuffer->setup(100, 100);

		viewer.init(this);

		SpEventDispatcher::instance()->addKeyboardListener(this);

		SpPhysicSettings::instance()->disableSimulation();

		// init renderer 
		SpWorldManagerInstance->current()->renderer = sp_mem_new(OpenGLRendererManager)();
		SpWorldManagerInstance->current()->renderer->init(&viewer);
		SpWorldManagerInstance->current()->renderer->resize((sp_float)width(), (sp_float)width());

		// init lights
		SpLightManager::instance()->addLight(SP_RGB_WHITE, Vec3(10.0f, 10.0f, 0.0f));

		const sp_uint rockLength = SpWorldManagerInstance->current()->objectsLengthAllocated() - 1u;
		const sp_uint worldObjectsLength = 1u;
		
		worldObjects = sp_mem_new(WorldObjectList)(worldObjectsLength);
		worldObjects->init();
		SpWorldManagerInstance->current()->renderer->addGraphicObject(worldObjects);

		const Vec3 rockScale(2.0f, 2.0f, 2.0f);

		rockList = sp_mem_new(RockList)(rockLength);
		rockList->translate(0u, { 0.0f, 10.0f, 0.0f });
		rockList->scale(0u, rockScale);
		rockList->rotate(0u, Quat::createRotate(degreesToRadians(10), Vec3(1.0f, 0.0f, 0.0f)));
		
		//const sp_uint boxSize = 20u;
		//const sp_uint boxSize = 50u;
		//Randomizer rand(0, boxSize);
		//sp_uint halfSpace = divideBy2(boxSize);

		SpCSVFileReader csvReader("seed.csv");

		for (register sp_uint i = 1u; i < rockLength; i++)
		{
			sp_char value[30];

			csvReader.readValue(value, 30);
			const sp_float orientationX = convert(value);
			rockList->rotate(i, Quat::createRotationAxisX(orientationX));

			csvReader.readValue(value, 30);
			const sp_float orientationY = convert(value);
			rockList->rotate(i, Quat::createRotationAxisY(orientationY));

			csvReader.readValue(value, 30);
			const sp_float orientationZ = convert(value);
			rockList->rotate(i, Quat::createRotationAxisZ(orientationZ));

			//const sp_float x = (sp_float)rand.randInt();
			//const sp_float z = (sp_float)rand.randInt();
			//rockList->translate(i, { x - halfSpace, multiplyBy2(i) + 10.0f, z - halfSpace });
			rockList->translate(i, { 0.0f, multiplyBy16(i) + 10.0f, 0.0f });

			rockList->scale(i, rockScale);
		}

		rockList->init();

		SpWorldManagerInstance->current()->renderer->addGraphicObject(rockList);
	}

	void GameFrame::update(sp_float elapsedTime)
	{
		if (!SpPhysicSettings::instance()->isSimulationEnabled())
			return;

		for (sp_uint i = 0u; i < worldObjects->length(); i++)
			worldObjects->update(i, elapsedTime);

		SpPhysicSettings* settings = SpPhysicSettings::instance();
		
		for (sp_uint i = 0u; i < rockList->length(); i++)
		{
			rockList->rigidBody3D(i)->currentState.addForce(settings->gravityForce());
			rockList->update(i, elapsedTime);
		}
	}

	void GameFrame::renderGUI()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("Framebuffer", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		loadState();

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImVec2 size = ImVec2((sp_float)width(), (sp_float)height());
		SpWorldManagerInstance->current()->renderer->resize(size.x, size.y);
		
		ImGui::Image((void*)(intptr_t)framebuffer->colorTexture(), size, ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::Image((void*)(intptr_t)framebuffer->colorTexture(), size, ImVec2(1, 0), ImVec2(0, 1));
		//ImGui::Image((void*)(intptr_t)framebuffer->colorTexture(), size, ImVec2(1, 1), ImVec2(0, 0));
		//ImGui::Image((void*)(intptr_t)framebuffer->colorTexture(), size, ImVec2(0, 0), ImVec2(1, 1));

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Camera"))
			{
				// camera frame!
			}

			if (ImGui::MenuItem("Wireframe"))
			{
				// camera frame!
			}

			if (ImGui::MenuItem("Background Color"))
			{
				// camera frame!
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void GameFrame::preRender()
	{
	}

	void GameFrame::render()
	{
		const SpViewportData viewport = SpWorldManagerInstance->current()->renderer->viewport();

		if (viewport.width != framebuffer->size().width
			|| viewport.height != framebuffer->size().height)
			framebuffer->setup(viewport.width, viewport.height);
		
		framebuffer->use();

		// TODO: REMOVE
		Timer t;
		t.start();
		SpWorldManagerInstance->current()->renderer->render();
		((sp_float*)SpGlobalPropertiesInscance->get(ID_renderingTime))[0] = t.elapsedTime();

		framebuffer->disable();
	}

	void GameFrame::onKeyboardEvent(SpKeyboardEvent* evt)
	{
		if (evt->type == SpKeyEventType::Released)
			return;

		const sp_float temp = 1.0f;
		const sp_uint objectIndex = 0u;

		switch (evt->key)
		{
		case SP_KEYBOARD_KEY_Q:
		{
			const Vec3 position = rockList->rigidBody3D(objectIndex)->currentState.position()
				+ Vec3(0.0f, 0.5f, 0.0f);
			const Vec3 force = Vec3(0.0f, 0.0f, -0.1f);
			rockList->rigidBody3D(objectIndex)->addImpulseAngular(position, force);
			break;
		}
		case SP_KEYBOARD_KEY_A:
		{
			rockList->rigidBody3D(objectIndex)->addImpulse(Vec3(-temp, 0.0f, 0.0f));
			break;
		}
		case SP_KEYBOARD_KEY_D:
		{
			rockList->rigidBody3D(objectIndex)->addImpulse(Vec3(temp, 0.0f, 0.0f));
			break;
		}
		case SP_KEYBOARD_KEY_W:
		{
			rockList->rigidBody3D(objectIndex)->addImpulse(Vec3(0.0f, 0.0f, -temp));
			break;
		}
		case SP_KEYBOARD_KEY_S:
		{
			rockList->rigidBody3D(objectIndex)->addImpulse(Vec3(0.0f, 0.0f, temp));
			break;
		}
		case SP_KEYBOARD_KEY_F5:
		{
			if (SpPhysicSettings::instance()->isSimulationEnabled())
				SpPhysicSettings::instance()->disableSimulation();
			else
				SpPhysicSettings::instance()->enableSimulation();
			
			break;
		}
		case SP_KEYBOARD_KEY_SPACE:
			static int rot = 2;
			Quat q = Quat::createRotationAxisZ(degreesToRadians(rot));
			rockList->rotate(0u, q);
			//rot++;
			break;

		}
	}

	void GameFrame::dispose()
	{
		if (framebuffer != nullptr)
		{
			sp_mem_delete(framebuffer, OpenGLFramebuffer);
			framebuffer = nullptr;
		}

		if(gridSystem != nullptr)
		{
			sp_mem_delete(gridSystem, GridSystem);
			gridSystem = nullptr;
		}

		if (rockList != nullptr)
		{
			sp_mem_delete(rockList, RockList);
			rockList = nullptr;
		}
	}
}