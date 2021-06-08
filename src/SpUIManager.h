#ifndef SP_UI_MANAGER_HEADER
#define SP_UI_MANAGER_HEADER

#define GLEW_STATIC

#include "SpectrumFronend.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SpToolbarFrame.h"
#include "SpStatusBarFrame.h"
#include "GameFrame.h"
#include "SpEventDispatcher.h"
#include "SpWindow.h"
#include "RendererEditor.h"
#include "NotificationFrame.h"
#include "SpUIIcons.h"
#include "SpUIMenuBarComponent.h"
#include "SpUINotificationManager.h"

namespace NAMESPACE_FRONTEND
{	
	class SpUIManager :
		public RendererEditor,
		public SpWindowEventListener
	{
	private:
		SpToolbarFrame toolbarFrame;
		SpStatusBarFrame statusBarFrame;
		GameFrame gameFrame;

		SpUIMenuBarComponent mainMenuBar;

	public:
		SpWindow* window = nullptr;
		SpVector<SpFrame*> frames;

		API_INTERFACE void init() override
		{
			SpEventDispatcher::instance()->addWindowListener(this);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigWindowsMoveFromTitleBarOnly = true;

			SpUIFonts::init();

#ifdef OPENGL_ENABLED
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->handler(), true);
			const sp_char* glsl_version = "#version 130";
			ImGui_ImplOpenGL3_Init(glsl_version);
#endif

			SpUIIcons::init();
			SpUINotificationManager::init();

			frames.add(&toolbarFrame);
			frames.add(&statusBarFrame);
			frames.add(&gameFrame);
			
			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->init(window);

			mainMenuBar.init(window);
		}

		API_INTERFACE void update(sp_float elapsedTime) override
		{
			gameFrame.update(elapsedTime);
		}

		API_INTERFACE void preRender() override
		{
			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->preRender();
		}

		API_INTERFACE void renderGUI() override
		{
			glViewport(0, 0, window->state()->width, window->state()->height);
			glScissor(0, 0, window->state()->width, window->state()->height);
			glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			mainMenuBar.render();

			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->renderGUI();

			SpUINotificationManagerInstance->render();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		API_INTERFACE void render() override
		{
			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->render();
		}

		API_INTERFACE void postRender() override
		{
			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->postRender();
		}

		API_INTERFACE void onWindowEvent(SpWindowEvent* evt) override
		{
			switch (evt->type)
			{
			case (sp_uint)SpWindowEventType::Resized:
				//renderer->resize((sp_float)width, (sp_float)height);
				break;

			default:
				break;
			}
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Main Frame";
		}

		API_INTERFACE static void initialize();

		API_INTERFACE static void release();

		API_INTERFACE void dispose() override
		{
			if (frames.length() > ZERO_UINT)
			{
				for (SpVectorItem<SpFrame*>* item = frames.last(); item != NULL; item = item->previous())
					item->value()->dispose();
				frames.clear();

				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplGlfw_Shutdown();
				ImGui::DestroyContext();
			}
		}

		~SpUIManager()
		{
			dispose();
		}

	};

	extern SpUIManager* SpUIManagerInstance;
}

#endif // SP_UI_MANAGER_HEADER