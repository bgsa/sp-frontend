#include "MainFrame.h"

namespace NAMESPACE_FRONTEND
{
	void MainFrame::setWindow(GLFWwindow* window)
	{
		this->window = window;
	}

	void MainFrame::init(NAMESPACE_RENDERING::IRendererManager* renderer)
	{
		this->renderer = renderer;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		//io.Fonts->AddFontFromFileTTF("../resources/fonts/16x8pxl-mono.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
		//io.Fonts->AddFontFromFileTTF("../resources/fonts/Dashboard-Regular.otf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
		//io.Fonts->AddFontFromFileTTF("../resources/fonts/DukasCFRegular-Regular.otf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());
		//io.Fonts->AddFontFromFileTTF("../resources/fonts/DukasCFSemiBold-Regular.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());

		ImGui_ImplGlfw_InitForOpenGL(window, true);

		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);

		aboutFrame.init();
		projectExplorerFrame.init();
		propertiesFrame.init();
		logFrame.init();

		texture = NAMESPACE_RENDERING::OpenGLTexture::createFromFramebuffer();
	}

	void MainFrame::update()
	{

	}

	void MainFrame::renderMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Quit", NULL))
					glfwSetWindowShouldClose(window, true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Camera", NULL);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Project Explorer", NULL, projectExplorerFrame.isVisible()))
					if (projectExplorerFrame.isVisible())
						projectExplorerFrame.hide();
					else
						projectExplorerFrame.show();

				if (ImGui::MenuItem("Properties", NULL, propertiesFrame.isVisible()))
					if (propertiesFrame.isVisible())
						propertiesFrame.hide();
					else
						propertiesFrame.show();

				if (ImGui::MenuItem("Log", NULL, logFrame.isVisible()))
					if (logFrame.isVisible())
						logFrame.hide();
					else
						logFrame.show();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::BeginMenu("Theme"))
				{
					if (ImGui::MenuItem("Dark (Default)"))
						ImGui::StyleColorsDark();

					if (ImGui::MenuItem("Light"))
						ImGui::StyleColorsLight();

					if (ImGui::MenuItem("Classic"))
						ImGui::StyleColorsClassic();

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About Spectrum", NULL, aboutFrame.isVisible()))
					if (aboutFrame.isVisible())
						aboutFrame.hide();
					else
						aboutFrame.show();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void MainFrame::render(const NAMESPACE_RENDERING::RenderData& renderData)
	{
		Vec2f imageSize = NAMESPACE_RENDERING::RendererSettings::getInstance()->getSize();
		sp_uchar* data = NAMESPACE_RENDERING::Framebuffer::getFramebuffer();
		texture->updateData(data);
		delete[] data;

		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		renderMainMenuBar();

		aboutFrame.render();
		projectExplorerFrame.render();
		propertiesFrame.render();
		logFrame.render();

		ImGui::Begin("Game Framebuffer");
		ImGui::Image((void*)(intptr_t)texture->getId(), ImVec2(imageSize.x, imageSize.y));
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void MainFrame::onResize(sp_int width, sp_int height)
	{
		renderer->resize((sp_float) width, (sp_float)height);
	}

	void MainFrame::onClose()
	{
		renderer->stop();
	}

	void MainFrame::dispose()
	{
		if (!isDisposed)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			isDisposed = true;
		}
	}
}