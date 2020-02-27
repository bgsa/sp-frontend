#include "MainFrame.h"

bool show_another_window = false;
static bool a = false;

void MainFrame::init(GLFWwindow* window)
{
    this->window = window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    //io.Fonts->AddFontFromFileTTF("../resources/fonts/Dashboard-Regular.otf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    io.Fonts->AddFontFromFileTTF("../resources/fonts/16x8pxl-mono.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    //io.Fonts->AddFontFromFileTTF("../resources/fonts/DukasCFRegular-Regular.otf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());
    //io.Fonts->AddFontFromFileTTF("../resources/fonts/DukasCFSemiBold-Regular.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
}

void MainFrame::preRender()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if( ImGui::MenuItem("Quit", NULL, &a)) 
	            glfwSetWindowShouldClose(window, true);

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::MenuItem("Camera", NULL, &a);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help"))
        {
            if( ImGui::MenuItem("About Spectrum", NULL, aboutFrame.isVisible()))
                if ( aboutFrame.isVisible() )
                    aboutFrame.hide();
                else
                    aboutFrame.show();
                    
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    aboutFrame.render();

    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
}

void MainFrame::render()
{
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void MainFrame::release()
{
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
