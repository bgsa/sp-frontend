#include "GameFrame.h"

namespace NAMESPACE_FRONTEND
{

	void GameFrame::init(SpWindow* window)
	{
		SpFrame::init(window);

		viewer.init(this);

		// init renderer 
		renderer = sp_mem_new(OpenGLRendererManager)();
		renderer->init(&viewer);
		renderer->resize((sp_float)width(), (sp_float)width());

		// init objects to render
		gridSystem = sp_mem_new(GridSystem)();
		gridSystem->init();
		renderer->addGraphicObject(gridSystem);

		rockRenderer = sp_mem_new(RockRenderer)();

		rock1 = sp_mem_new(Rock)();
		rock1->init();
		rock1->setRenderer(rockRenderer);
		rock1->translate(10.0f, 0.0f, 0.0f);
		rockRenderer->setObjects(rock1, ONE_UINT);
		renderer->addGraphicObject(rock1);

		rock2 = sp_mem_new(Rock)();
		rock2->init();
		rock2->setRenderer(rockRenderer);
		rockRenderer->setObjects(rock2, ONE_UINT);
		renderer->addGraphicObject(rock2);


		Mat4f defaultScale = Mat4f::createScale(2.8f, 2.9f, 2.6f);
		Mat4f defaultTranslation = Mat4f::createTranslate(0.2f, 1.0f, 1.3f);

		kdops = sp_mem_new(kDOP18List)();
		kdops->setLength(2);
		kdops->transforms()[0] = defaultTranslation * Mat4f::createTranslate(10.0f, 0.0f, 0.0f) * defaultScale;
		kdops->transforms()[1] = defaultTranslation * defaultScale;
		kdops->init();
		renderer->addGraphicObject(kdops);

		boundingVolumeRenderer = sp_mem_new(RendererList<DOP18>)();
		boundingVolumeRenderer->setList(kdops);
		kdops->setRenderer(boundingVolumeRenderer);
		boundingVolumeRenderer->init();
		
		texture = OpenGLTexture::createFromFramebuffer();
	}

	void GameFrame::update(sp_longlong elapsedTime)
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
		texture->use()->setData(data, Vec2i(viewport->width, viewport->height), GL_RGBA);
		sp_mem_release(data);

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
		
		if (renderer != nullptr)
		{
			sp_mem_delete(renderer, OpenGLRendererManager);
			renderer = nullptr;
		}
		
	}
}