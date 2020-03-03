#ifndef MAIN_WINDOW_HEADER
#define MAIN_WINDOW_HEADER

#include <SpectrumFoundation.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "AboutFrame.h"
#include "ProjectExplorerFrame.h"
#include "LogFrame.h"
#include "PropertiesFrame.h"

class MainFrame
{
    private:
        GLFWwindow* window = NULL;

        AboutFrame aboutFrame;
        ProjectExplorerFrame projectExplorerFrame;
        LogFrame logFrame;
        PropertiesFrame propertiesFrame;

        void renderMainMenuBar();

    public:

    API_INTERFACE void init(GLFWwindow* window);

    API_INTERFACE void preRender();

    API_INTERFACE void render();

    API_INTERFACE void release();

};

#endif // MAIN_WINDOW_HEADER