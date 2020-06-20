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
		SpLightManager::instance()->init(1u);
		SpLightManager::instance()->lights(0u)->position(Vec3(10.0f, 10.0f, 0.0f));

		// init objects to render
		gridSystem = sp_mem_new(GridSystem)();
		gridSystem->init();
		renderer->addGraphicObject(gridSystem);

		//const sp_uint rockLength = 131072;
		const sp_uint rockLength = 3u;
		const sp_uint worldObjectsLength = 1u;
		Randomizer rand(0, 10000);
		sp_uint halfSpace = 10000 / 200;

		worldObjects = sp_mem_new(WorldObjectList)(worldObjectsLength);
		worldObjects->scale(0u, Vec3(100.0f, 1.0f, 100.0f));
		worldObjects->init();
		renderer->addGraphicObject(worldObjects);

		rockList = sp_mem_new(RockList)(rockLength);
		rockList->translate(0u, { 1.0f, 0.5f, 0.0f });
		rockList->translate(1u, { -10.0f, 10.5f, 0.0f });
		rockList->translate(2u, { 10.0f, 10.0f, 4.0f });
		for (sp_uint i = 3; i < rockLength; i++)
		{
			sp_float x = rand.rand() / 100.0f;
			sp_float y = rand.rand() / 100.0f;
			sp_float z = rand.rand() / 100.0f;

			rockList->translate(i, { x - halfSpace, 3.7f, z - halfSpace });
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
		
		for (sp_uint i = 0u; i < rockList->length(); i++)
			rockList->update(i, elapsedTime);
	}

	void GameFrame::renderGUI()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		
		ImGui::Begin("Game Framebuffer", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		loadState();

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImVec2 size = ImVec2((sp_float)width(), (sp_float)height());
		renderer->resize(size.x, size.y);

		ImGui::Image((void*)(intptr_t)texture->getId(), size, ImVec2(0, 1), ImVec2(1, 0));

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

		sp_uchar* data = Framebuffer::getFramebuffer();
		texture->use()->setData(data, SpSize<sp_int>(viewport->width, viewport->height), GL_RGBA);
		sp_mem_release(data);
	}

	void GameFrame::onKeyboardEvent(SpKeyboardEvent* evt)
	{
		const sp_float temp = 3.0f;

		switch (evt->key)
		{
		case SP_KEYBOARD_KEY_A:
		{
			rockList->physicProperties(1u)->addImpulse(Vec3(-temp, 0.0f, 0.0f));
			//rockList->physicProperties(1u)->addForce(Vec3(-temp, 0.0f, 0.0f));
			break;
		}
		case SP_KEYBOARD_KEY_D:
		{
			rockList->physicProperties(1u)->addImpulse(Vec3(temp, 0.0f, 0.0f));
			//rockList->physicProperties(1u)->addForce(Vec3(temp, 0.0f, 0.0f));
			break;
		}
		case SP_KEYBOARD_KEY_W:
		{
			rockList->physicProperties(1u)->addImpulse(Vec3(0.0f, 0.0f, -temp));
			//rockList->physicProperties(1u)->addForce(Vec3(0.0f, 0.0f, -temp));
			break;
		}
		case SP_KEYBOARD_KEY_S:
		{
			rockList->physicProperties(1u)->addImpulse(Vec3(0.0f, 0.0f, temp));
			//rockList->physicProperties(1u)->addForce(Vec3(0.0f, 0.0f, temp));
			break;
		}
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