#include "GameFrame.h"

namespace NAMESPACE_FRONTEND
{
	void GameFrame::init(SpWindow* window)
	{
		SpFrame::init(window);

		viewer.init(this);

		SpEventDispatcher::instance()->addKeyboardListener(this);
		SpEventDispatcher::instance()->addCollisionListener(this);

		// init renderer 
		renderer = sp_mem_new(OpenGLRendererManager)();
		renderer->init(&viewer);
		renderer->resize((sp_float)width(), (sp_float)width());

		// init objects to render
		gridSystem = sp_mem_new(GridSystem)();
		gridSystem->init();
		renderer->addGraphicObject(gridSystem);

		//const sp_uint rockLength = 131072;
		const sp_uint rockLength = 64;
		Randomizer rand(0, 10000);
		sp_uint halfSpace = 10000 / 200;

		rockList = sp_mem_new(RockList)(rockLength);
		rockList->translate(0u, { 0.0f, 0.7f, 0.0f });
		for (sp_uint i = 1; i < rockLength; i++)
		{
			sp_float x = rand.rand() / 100.0f;
			sp_float y = rand.rand() / 100.0f;
			sp_float z = rand.rand() / 100.0f;

			rockList->translate(i, { x - halfSpace, 0.7f, z - halfSpace });
		}

		rockList->init();
		rockList->setRenderer(rockRenderer);
		renderer->addGraphicObject(rockList);
		
		texture = OpenGLTexture::createFromFramebuffer();
	}

	void GameFrame::onCollisionEvent(SpCollisionEvent* evt)
	{
		sp_uint objIndex1 = evt->indexBody1;
		sp_uint objIndex2 = evt->indexBody1;
	}

	void GameFrame::update(sp_float elapsedTime)
	{
		renderer->update(elapsedTime);
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
		const sp_float temp = 1.0f;

		switch (evt->key)
		{
		case SP_KEYBOARD_KEY_A:
		{
			rockList->translate(0u, { temp * gameVelocity , 0.0f, 0.0f });
			break;
		}
		case SP_KEYBOARD_KEY_D:
		{
			rockList->translate(0u, { -temp * gameVelocity , 0.0f, 0.0f });
			break;
		}
		case SP_KEYBOARD_KEY_W:
			rockList->translate(0u, { 0.0f , 0.0f, temp * gameVelocity });
			break;

		case SP_KEYBOARD_KEY_S:
			rockList->translate(0u, { 0.0f , 0.0f, -temp * gameVelocity });
			break;
		}

		/*
		DOP18* bv1 = (DOP18*)rock1->boundingVolume();
		DOP18* bv2 = (DOP18*)rock2->boundingVolume();
		CollisionStatus status = bv1->collisionStatus(*bv2);

		if (status != CollisionStatus::OUTSIDE)
			Log::info("COLLISION!");
			*/
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

		if (rock1 != nullptr)
		{
			sp_mem_delete(rock1, Rock);
			rock1 = nullptr;
		}
		if (rock2 != nullptr)
		{
			sp_mem_delete(rock2, Rock);
			rock2 = nullptr;
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