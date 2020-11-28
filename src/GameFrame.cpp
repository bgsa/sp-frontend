#include "GameFrame.h"

namespace NAMESPACE_FRONTEND
{
	void GameFrame::init(SpWindow* window)
	{
		SpFrame::init(window);

		viewer.init(this);

		SpEventDispatcher::instance()->addKeyboardListener(this);

		SpPhysicSettings::instance()->disableSimulation();

		// init renderer 
		renderer = sp_mem_new(OpenGLRendererManager)();
		renderer->init(&viewer);
		renderer->resize((sp_float)width(), (sp_float)width());

		// init lights
		SpLightManager::instance()->addLight(SP_RGB_WHITE, Vec3(10.0f, 10.0f, 0.0f));

		const sp_uint rockLength = SpPhysicSimulator::instance()->objectsLengthAllocated() - 1u;
		const sp_uint worldObjectsLength = 1u;
		
		worldObjects = sp_mem_new(WorldObjectList)(worldObjectsLength);
		worldObjects->init();
		renderer->addGraphicObject(worldObjects);

		rockList = sp_mem_new(RockList)(rockLength);
		rockList->translate(0u, { 30.0f, 100.0f, 0.0f });
		rockList->rotate(0u, Quat::createRotate(degreesToRadians(30), Vec3(1.0f, 0.0f, 0.0f)));
		rockList->translate(1u, { -1.0f, 10.0f, 0.0f });
		rockList->translate(1u, { 4.0f, 100.7f, 4.0f });

		/*
		sp_uint index = ZERO_UINT;
		for (sp_uint y = 1u; y < 11u; y++)
		{
			for (sp_uint z = 1u; z < 11u; z++)
			{
				for (sp_uint x = 1u; x < 13u; x++)
				{
					rockList->translate(index, { x * 5u - 25.0f, y * 20.0f, z * 5u - 10.0f });
					index++;

					if (index == rockLength)
						goto keep_executing;
				}
			}
		}
	keep_executing:
		index = 0u;
		Vec3 translation = Vec3(20.0f, 30.0f, 0.0f);
		SpTransform* transformation = rockList->transforms(index);
		translation -= transformation->position;
		rockList->translate(index, translation);

		index = 1u;
		translation = Vec3(30.0f, 40.0f, 10.0f);
		transformation = rockList->transforms(index);
		translation -= transformation->position;
		rockList->translate(index, translation);

		index = 2u;
		translation = Vec3(10.0f, 45.0f, 1.0f);
		transformation = rockList->transforms(index);
		translation -= transformation->position;
		rockList->translate(index, translation);

		index = 3u;
		translation = Vec3(-5.0f, 45.0f, 9.0f);
		transformation = rockList->transforms(index);
		translation -= transformation->position;
		rockList->translate(index, translation);

		index = 4u;
		translation = Vec3(-15.0f, 15.0f, 5.0f);
		transformation = rockList->transforms(index);
		translation -= transformation->position;
		rockList->translate(index, translation);

		index = 5u;
		translation = Vec3(-25.0f, 60.0f, 11.0f);
		transformation = rockList->transforms(index);
		translation -= transformation->position;
		rockList->translate(index, translation);
		*/

		Randomizer rand(0, 3000);
		sp_uint halfSpace = 1000 / 200;
		const sp_uint randomizeFrom = 2u;
		if (rockLength > randomizeFrom)
		{
			for (sp_uint i = randomizeFrom; i < rockLength; i++)
			{
				sp_float x = rand.randInt() / 100.0f;
				sp_float y = rand.randInt() / 100.0f;
				sp_float z = rand.randInt() / 100.0f;

				// move the object to a random position
				rockList->translate(i, { x - halfSpace, y + 1.0f, z - halfSpace });

				// move away initial inter-penettrations
				DOP18* bvi = rockList->boundingVolumes(i);
				for (sp_uint j = 0; j < i; j++)
				{
					DOP18* bvj = rockList->boundingVolumes(j);
					CollisionStatus status = bvi->collisionStatus(*bvj);
					if (status != CollisionStatus::OUTSIDE)
					{
						i--; // back i index to translate a valid position
						break;
					}
				}
			}
		}

		rockList->init();
		rockList->setRenderer(rockRenderer);
		renderer->addGraphicObject(rockList);

		framebuffer = sp_mem_new(OpenGLFramebuffer)();
		framebuffer->setup(100, 100);
	}

	void GameFrame::update(sp_float elapsedTime)
	{
		if (!SpPhysicSettings::instance()->isSimulationEnabled())
			return;

		for (sp_uint i = 0u; i < worldObjects->length(); i++)
			worldObjects->update(i, elapsedTime);

		SpPhysicSettings* settings = SpPhysicSettings::instance();
		const Vec3 gravityForce = settings->gravityForce();
		
		for (sp_uint i = 0u; i < rockList->length(); i++)
		{
			rockList->physicProperties(i)->currentState.addForce(gravityForce);
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
		renderer->resize(size.x, size.y);
		
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
		SpViewportData* viewport = renderer->viewport();

		if (viewport->width != framebuffer->size().width
			|| viewport->height != framebuffer->size().height)
			framebuffer->setup(viewport->width, viewport->height);
		
		framebuffer->use();

		renderer->render();

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
			const Vec3 position = rockList->physicProperties(objectIndex)->currentState.position()
				+ Vec3(0.0f, 0.5f, 0.0f);
			const Vec3 force = Vec3(0.0f, 0.0f, -0.1f);
			rockList->physicProperties(objectIndex)->addImpulseAngular(position, force);
			break;
		}
		case SP_KEYBOARD_KEY_A:
		{
			rockList->physicProperties(objectIndex)->addImpulse(Vec3(-temp, 0.0f, 0.0f));
			break;
		}
		case SP_KEYBOARD_KEY_D:
		{
			rockList->physicProperties(objectIndex)->addImpulse(Vec3(temp, 0.0f, 0.0f));
			break;
		}
		case SP_KEYBOARD_KEY_W:
		{
			rockList->physicProperties(objectIndex)->addImpulse(Vec3(0.0f, 0.0f, -temp));
			break;
		}
		case SP_KEYBOARD_KEY_S:
		{
			rockList->physicProperties(objectIndex)->addImpulse(Vec3(0.0f, 0.0f, temp));
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

	void GameFrame::postRender()
	{
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

		if (rockRenderer != nullptr)
		{
			sp_mem_delete(rockRenderer, RockRenderer);
			rockRenderer = nullptr;
		}

		if (rockList != nullptr)
		{
			sp_mem_delete(rockList, RockList);
			rockList = nullptr;
		}
		
		if (renderer != nullptr)
		{
			sp_mem_delete(renderer, OpenGLRendererManager);
			renderer = nullptr;
		}
	}
}