#ifndef MAIN_WINDOW_HEADER
#define MAIN_WINDOW_HEADER

#include <apollo.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl2.h"
#include "AboutFrame.h"

class MainFrame
{
    private:
        GLFWwindow* window = NULL;
        AboutFrame aboutFrame;

    public:

    API_INTERFACE void init(GLFWwindow* window);

    API_INTERFACE void preRender();

    API_INTERFACE void render();

    API_INTERFACE void release();

};

#endif // MAIN_WINDOW_HEADER