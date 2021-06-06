#ifndef SP_UI_MANAGER_HEADER
#define SP_UI_MANAGER_HEADER

#define GLEW_STATIC

#include "SpectrumFronend.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "AboutFrame.h"
#include "ProjectExplorerFrame.h"
#include "LogFrame.h"
#include "PropertiesFrame.h"
#include "SpToolbarFrame.h"
#include "SpStatusBarFrame.h"
#include "GameFrame.h"
#include "SpEventDispatcher.h"
#include "SpWindow.h"
#include "RendererEditor.h"
#include "NewProjectFrame.h"
#include "NotificationFrame.h"
#include "SpUIIcons.h"
#include "addons/FileBrowser/ImGuiFileBrowser.h"

namespace NAMESPACE_FRONTEND
{
#define SP_UI_MAX_NOTIFICATION 10
#define SP_UI_NOTIFICATION_LIFETIME 200

	extern sp_bool showLoadProject;
	
	void loadProjectDialog_OnClose();

	class SpUIManager :
		public RendererEditor,
		public SpWindowEventListener
	{
	private:
		SpWindow* window = NULL;
		
		AboutFrame aboutFrame;
		ProjectExplorerFrame projectExplorerFrame;
		LogFrame logFrame;
		PropertiesFrame propertiesFrame;
		SpToolbarFrame toolbarFrame;
		SpStatusBarFrame statusBarFrame;
		GameFrame gameFrame;
		NewProjectFrame newProjectFrame;
		NotificationFrame notifications[SP_UI_MAX_NOTIFICATION];
		sp_ushort _notificationsLength;
		sp_uint notificationIndex;

		imgui_addons::ImGuiFileBrowser loadProjectDialog;

		inline void renderMainMenuBar()
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New Project", NULL))
						newProjectFrame.show();

					if (ImGui::MenuItem("Save", "CTRL+S", false, SpProjectManagerInstance->isLoaded()))
						SpProjectManagerInstance->save();

					if (ImGui::MenuItem("Load", "", false))
						showLoadProject = true;

					if (ImGui::MenuItem("Quit", NULL))
						window->close();

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
		
			if (showLoadProject)
			{
				ImGui::OpenPopup("Load Project##popup");
				if (loadProjectDialog.showFileDialog("Load Project##popup", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(600, 300), SP_FILENAME_PROJECT_SUFFIX))
				{
					sp_char filename[512];
					strReplace(loadProjectDialog.selected_path.c_str(), '/', SP_DIRECTORY_SEPARATOR, filename);

					SpProjectManagerInstance->load(filename);
					showLoadProject = false;
				}
			}

		}

	public:
		SpVector<SpFrame*> frames;

		API_INTERFACE void setWindow(SpWindow* window)
		{
			this->window = window;
		}

		API_INTERFACE void init() override
		{
			SpEventDispatcher::instance()->addWindowListener(this);

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigWindowsMoveFromTitleBarOnly = true;

			//io.Fonts->AddFontFromFileTTF("../resources/fonts/16x8pxl-mono.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
			//io.Fonts->AddFontFromFileTTF("../resources/fonts/Dashboard-Regular.otf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
			//io.Fonts->AddFontFromFileTTF("../resources/fonts/DukasCFRegular-Regular.otf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());
			//io.Fonts->AddFontFromFileTTF("../resources/fonts/DukasCFSemiBold-Regular.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());

#ifdef OPENGL_ENABLED
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->handler(), true);
			const sp_char* glsl_version = "#version 130";
			ImGui_ImplOpenGL3_Init(glsl_version);
#endif

			SpUIIcons::init();

			frames.add(&aboutFrame);
			frames.add(&toolbarFrame);
			frames.add(&projectExplorerFrame);
			frames.add(&propertiesFrame);
			frames.add(&logFrame);
			frames.add(&statusBarFrame);
			frames.add(&gameFrame);
			frames.add(&newProjectFrame);

			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->init(window);

			// Init all notifications
			notificationIndex = ZERO_UINT;
			_notificationsLength = ZERO_USHORT;
			for (sp_int i = 0; i < SP_UI_MAX_NOTIFICATION; i++)
			{
				notifications[i].id = i;
				notifications[i].init(window);
			}

			propertiesFrame.selectedObject(1u);

			loadProjectDialog.onClose = &loadProjectDialog_OnClose;
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

			renderMainMenuBar();

			for (SpVectorItem<SpFrame*>* item = frames.begin(); item != NULL; item = item->next())
				item->value()->renderGUI();

			sp_int idx = ZERO_INT;
			for (sp_int i = 0; i < SP_UI_MAX_NOTIFICATION; i++)
				if (notifications[i].isVisible())
				{
					notifications[i].index = idx++;
					notifications[i].renderGUI();
				}

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

		API_INTERFACE inline sp_ushort notificationsLength() const
		{
			return _notificationsLength;
		}

		API_INTERFACE inline sp_int addNotification(const sp_char* message, const NotificationType type, const sp_uint lifetime = SP_UI_NOTIFICATION_LIFETIME)
		{
			const sp_size messageLength = std::strlen(message);

			std::memcpy(notifications[notificationIndex].message, message, messageLength);

			notifications[notificationIndex].lifetime = lifetime;
			notifications[notificationIndex].type = type;
			notifications[notificationIndex].show();

			if (notificationIndex + 1 == SP_UI_MAX_NOTIFICATION)
				notificationIndex = ZERO_UINT;
			else
				notificationIndex++;

			_notificationsLength++;

			return notifications[notificationIndex].id;
		}

		API_INTERFACE inline void removeNotification(const sp_int id)
		{
			notifications[id].hide();
			_notificationsLength--;
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