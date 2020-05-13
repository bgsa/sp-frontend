#include "GameFrame.h"

namespace NAMESPACE_FRONTEND
{

	void GameFrame::init(SpWindow* window)
	{
		SpFrame::init(window);

		viewer.init(this);

		renderer = sp_mem_new(DefaultRendererManager)();
		renderer->init(&viewer);
		renderer->resize((sp_float)width(), (sp_float)width());

		gridSystem = sp_mem_new(GridSystem)();
		gridSystem->init();
		renderer->addGraphicObject(gridSystem);

		rockRenderer = sp_mem_new(RockRenderer)();
		rock = sp_mem_new(Rock)();
		rock->init();
		rock->setRenderer(rockRenderer);
		rockRenderer->setObjects(rock, ONE_UINT);
		renderer->addGraphicObject(rock);

		texture = OpenGLTexture::createFromFramebuffer();
	}

	void GameFrame::update(sp_longlong elapsedTime)
	{
		renderer->update(elapsedTime);
	}

	void GameFrame::renderGUI()
	{
		ImGui::Begin("Game Framebuffer", NULL, ImGuiWindowFlags_NoScrollbar);
		loadState();

		ImVec2 size = ImVec2(width(), height());
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
		texture->use()->setData(data, Vec2i(viewport->width, viewport->height), GL_RGBA);
		sp_mem_release(data);

	}

	void GameFrame::postRender()
	{
	}

	void GameFrame::dispose()
	{
		if(gridSystem != NULL)
		{
			sp_mem_delete(gridSystem, GridSystem);
			gridSystem = NULL;
		}
	}
}