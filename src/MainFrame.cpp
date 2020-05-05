#include "MainFrame.h"

namespace NAMESPACE_FRONTEND
{
	void MainFrame::setWindow(GLFWwindow* window)
	{
		this->window = window;
	}

	void MainFrame::init(IRendererManager* renderer)
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

		gridSystem = sp_mem_new(GridSystem)();
		gridSystem->init();
		this->renderer->addGraphicObject(gridSystem);

		texture = OpenGLTexture::createFromFramebuffer();
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

	void MainFrame::preRender()
	{
		renderer->resize(gameFrambufferSize.x, gameFrambufferSize.y);
		texture->resize(Vec2i((sp_int)gameFrambufferSize.x, (sp_int)gameFrambufferSize.y));
	}

	void MainFrame::render(const RenderData& renderData)
	{
		Vec2f size = RendererSettings::getInstance()->getSize();
		sp_uchar* data = Framebuffer::getFramebuffer();
		texture->use()->setData(data, Vec2i((sp_int)size.x, (sp_int)size.y), GL_RGBA);
		sp_mem_release(data);

		sp_int width, height;
		glfwGetWindowSize(window, &width, &height);
		renderer->resize((sp_float)width, (sp_float)height);

		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glViewport(0, 0, width, height);
		glScissor(0, 0, width, height);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		renderMainMenuBar();

		aboutFrame.render();
		projectExplorerFrame.render();
		propertiesFrame.render();
		logFrame.render();

		ImGui::Begin("Game Framebuffer");
			gameFrambufferSize = ImGui::GetWindowSize();
			ImGui::Image((void*)(intptr_t)texture->getId(), gameFrambufferSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void MainFrame::postRender()
	{
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
			if(gridSystem != NULL)
			{
				sp_mem_delete(gridSystem, GridSystem);
				gridSystem = NULL;
			}

			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			isDisposed = true;
		}
	}
}