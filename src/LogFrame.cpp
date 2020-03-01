#include "LogFrame.h"

void LogFrame::init()
{
    resize(500, 200);
    show();

	char text1[7] = "teste1";
	char text2[7] = "teste2";

    messages.push_back(text1);
    messages.push_back(text2);
}

void LogFrame::render()
{
    if ( ! isVisible() )
        return;

    if ( ImGui::Begin("Log Window", NULL, ImGuiWindowFlags_NoCollapse) )
    {
        ImGui::SetWindowSize(ImVec2((sp_float)getWidth(), (sp_float)getHeight()));

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