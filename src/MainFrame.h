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

class MainFrame : 
	public NAMESPACE_RENDERING::RendererEditor,
	public WindowInputDeviceHandler
{
    private:
        GLFWwindow* window = NULL;
		NAMESPACE_RENDERING::OpenGLTexture* texture = NULL;

        AboutFrame aboutFrame;
        ProjectExplorerFrame projectExplorerFrame;
        LogFrame logFrame;
        PropertiesFrame propertiesFrame;

        void renderMainMenuBar();

    public:

	API_INTERFACE void setWindow(GLFWwindow* window);

    API_INTERFACE void init() override;

	API_INTERFACE void update() override;

    API_INTERFACE void render(const NAMESPACE_RENDERING::RenderData& renderData) override;

	API_INTERFACE void onClose() override
	{
		sp_int a = 1;
	}

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

#endif // MAIN_WINDOW_HEADER