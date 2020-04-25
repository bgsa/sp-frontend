#ifndef MAIN_WINDOW_HEADER
#define MAIN_WINDOW_HEADER

#define GLEW_STATIC

#include "SpectrumFronend.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "AboutFrame.h"
#include "ProjectExplorerFrame.h"
#include "LogFrame.h"
#include "PropertiesFrame.h"
#include "RendererEditor.h"
#include "RenderData.h"
#include "Framebuffer.h"
#include "RendererSettings.h"
#include "OpenGLTexture.h"
#include "WindowInputDeviceHandler.h"
#include "IRendererManager.h"
#include "GridSystem.h"

namespace NAMESPACE_FRONTEND
{
	class MainFrame :
		public RendererEditor,
		public WindowInputDeviceHandler
	{
	private:
		GLFWwindow* window = NULL;
		OpenGLTexture* texture = NULL;

		AboutFrame aboutFrame;
		ProjectExplorerFrame projectExplorerFrame;
		LogFrame logFrame;
		PropertiesFrame propertiesFrame;

		void renderMainMenuBar();

		GridSystem* gridSystem = NULL;
		ImVec2 gameFrambufferSize = { 800.0f, 600.0f };

		sp_bool isDisposed = false;

	public:

		API_INTERFACE void setWindow(GLFWwindow* window);

		API_INTERFACE void init(IRendererManager* renderer) override;

		API_INTERFACE void update() override;

		API_INTERFACE void preRender() override;

		API_INTERFACE void render(const RenderData& renderData) override;

		API_INTERFACE void postRender() override;

		API_INTERFACE void onClose() override;

		API_INTERFACE void onResize(sp_int width, sp_int height) override;

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Main Frame";
		}

		API_INTERFACE void dispose() override;

		~MainFrame()
		{
			dispose();
		}

	};
}

#endif // MAIN_WINDOW_HEADER