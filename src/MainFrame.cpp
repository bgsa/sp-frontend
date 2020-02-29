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

    aboutFrame.init();
    projectExplorerFrame.init();
}

void MainFrame::preRender()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderMainMenuBar();

    aboutFrame.render();
    projectExplorerFrame.render();

    ImGui::Render();
}

void MainFrame::renderMainMenuBar()
{
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

        if (ImGui::BeginMenu("Windows"))
        {
            if( ImGui::MenuItem("Project Explorer", NULL, projectExplorerFrame.isVisible()) )
                if( projectExplorerFrame.isVisible() )
                    projectExplorerFrame.hide();
                else
                    projectExplorerFrame.show();
                    
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
