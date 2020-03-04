#ifdef UNIX
    #define GL_SILENCE_DEPRECATION
#endif

#include "MainFrame.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
        return 1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Spectrum Engine", NULL, NULL);
    if (window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

	if (glewInit() != GLEW_OK)
		return -1;

    glfwSetWindowTitle(window, "Spectrum Engine");

    MainFrame mainFrame;
    mainFrame.init(window);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        mainFrame.preRender();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        mainFrame.render();

        //glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    mainFrame.release();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
