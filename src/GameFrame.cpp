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
		Randomizer rand(0, 5000);
		sp_uint halfSpace = 5000 / 200;

		worldObjects = sp_mem_new(WorldObjectList)(worldObjectsLength);
		worldObjects->scale(0u, Vec3(100.0f, 1.0f, 100.0f));
		worldObjects->init();
		renderer->addGraphicObject(worldObjects);

		rockList = sp_mem_new(RockList)(rockLength);
		rockList->translate(0u, { 2.0f, 10.0f, 3.0f });
		rockList->rotate(0u, Quat::createRotate(degreesToRadians(30), Vec3(0.0f, 0.0f, 0.0f)));

		//rockList->translate(1u, { 3.0f, 10.0f, 0.0f });
		//rockList->rotate(1u, Quat::createRotate(degreesToRadians(30), Vec3(0.0f, 0.0f, 1.0f)));

		const sp_uint randomizeFrom = 2u;
		if (rockLength > randomizeFrom)
		{
			for (sp_uint i = randomizeFrom; i < rockLength; i++)
			{
				sp_float x = rand.rand() / 100.0f;
				sp_float y = rand.rand() / 100.0f;
				sp_float z = rand.rand() / 100.0f;

				// move the object to a random position
				rockList->translate(i, { x - halfSpace, y + 3.7f, z - halfSpace });

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

		texture = OpenGLTexture::createFromFramebuffer();
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
		texture->resize(SpSize<sp_int>((sp_int)size.x, (sp_int)size.y));

		ImGui::Image((void*)(intptr_t)texture->id(), size, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Camera"))
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
		renderer->render();

		SpViewportData* viewport = renderer->viewport();

		sp_uchar* data = ALLOC_ARRAY(sp_uchar, FOUR_INT * viewport->width * viewport->height);
		Framebuffer::getFramebuffer(data);

		texture
			->use()
			->updateData(data, GL_RGBA);
		
		ALLOC_RELEASE(data);
	}

	void GameFrame::onKeyboardEvent(SpKeyboardEvent* evt)
	{
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
		if (texture != nullptr)
		{
			sp_mem_delete(texture, OpenGLTexture);
			texture = nullptr;
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