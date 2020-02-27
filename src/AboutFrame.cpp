#include "AboutFrame.h"

void AboutFrame::show()
{
    this->showWindow = true;
}

void AboutFrame::hide()
{
    this->showWindow = false;
}

sp_bool AboutFrame::isVisible()
{
    return this->showWindow;
}

void AboutFrame::render()
{
    if( ! showWindow ) 
        return;
        
    const ImVec2 windowSize = ImGui::GetIO().DisplaySize;
    const sp_int width = 300, height = 150;

    ImGui::Begin("About Spectrum Engine", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiCond_FirstUseEver);
    ImGui::SetWindowSize(ImVec2(width, height));
    ImGui::SetWindowPos( ImVec2( ( (sp_int)windowSize.x >> 1) - (width >> 1) , ((sp_int)windowSize.y >> 1) - (height >> 1) ) );
    ImVec2 itemSize = ImGui::GetItemRectSize();

    ImGui::Text("Version: 0.1.0");
    ImGui::Text("Release Date: 01/01/2020");

    ImGui::Indent(((sp_int)itemSize.x >> 1) - 60);
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    if ( ImGui::Button("Close", ImVec2(100, 25)) )
        showWindow = false;

    ImGui::End();

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
        showWindow = false;
}