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

    ImGui_ImplGlfw_InitForOpenGL(window, true);

	const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

    aboutFrame.init();
    projectExplorerFrame.init();
    propertiesFrame.init();
    logFrame.init();
}

void MainFrame::preRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderMainMenuBar();

    aboutFrame.render();
    projectExplorerFrame.render();
    propertiesFrame.render();
    logFrame.render();

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

            if( ImGui::MenuItem("Properties", NULL, propertiesFrame.isVisible()) )
                if( propertiesFrame.isVisible() )
                    propertiesFrame.hide();
                else
                    propertiesFrame.show();

            if( ImGui::MenuItem("Log", NULL, logFrame.isVisible()) )
                if( logFrame.isVisible() )
                    logFrame.hide();
                else
                    logFrame.show();
                    
            ImGui::EndMenu();
        }

        if( ImGui::BeginMenu("Settings") )
        {
            if( ImGui::BeginMenu("Theme") )
            {
                if( ImGui::MenuItem("Dark (Default)") )
                    ImGui::StyleColorsDark();

                if( ImGui::MenuItem("Light") )
                    ImGui::StyleColorsLight();

                if( ImGui::MenuItem("Classic") )
                    ImGui::StyleColorsClassic();

                ImGui::EndMenu();
            }

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
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainFrame::release()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
