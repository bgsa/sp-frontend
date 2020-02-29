#include "LogFrame.h"

void LogFrame::init()
{
    resize(500, 200);
    show();

    messages.push_back("teste1");
    messages.push_back("teste2");
}

void LogFrame::render()
{
    if ( ! isVisible() )
        return;

    if ( ImGui::Begin("Log Window", NULL, ImGuiWindowFlags_NoCollapse) )
    {
        ImGui::SetWindowSize(ImVec2(getWidth(), getHeight()));

        for (sp_uint i = 0 ; i < messages.size(); i++ )
            ImGui::Text(messages[i]);

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Clear"))
                messages.clear();

            ImGui::EndPopup();
        }

        ImGui::End();
    }

}