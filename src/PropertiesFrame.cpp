#include "PropertiesFrame.h"

void PropertiesFrame::init()
{
    resize(300, 600);
    show();
}

void PropertiesFrame::render()
{
    if ( ! isVisible() )
        return;

    setHeight(getWindowSize().y - 25);


    if ( ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoCollapse) )
    {
        ImGui::SetWindowSize(ImVec2(getWidth(), getHeight()));
        ImGui::SetWindowPos(ImVec2(getWindowSize().x - getWidth() , 25.0f));

        ImGui::Columns(2, "Bar");
        
        for(sp_uint i = 0 ; i < 3 ; i++ )
        {
            ImGui::Text("Label");
            ImGui::NextColumn();

            ImGui::Text("Value");
            ImGui::NextColumn();
        }



        ImGui::End();
    }

}