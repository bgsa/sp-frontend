#include "ProjectExplorerFrame.h"

void ProjectExplorerFrame::init()
{
    resize(300, (sp_int)getWindowSize().y - 25);
    show();    
}

void ProjectExplorerFrame::render()
{
    if ( ! isVisible() )
        return;

    setHeight((sp_int)getWindowSize().y - 25);

    ImGui::Begin("Project Explorer", NULL, ImGuiWindowFlags_NoCollapse);
    //ImGui::SetWindowSize(ImVec2(ImGui::GetItemRectSize().x, (sp_float) getHeight()));
    //ImGui::SetWindowPos( ImVec2( 0.0f , 25.0f ));

    ImGui::SetNextTreeNodeOpen(true);
    if (ImGui::TreeNode("project-1"))
    {
        ImGui::SetNextTreeNodeOpen(true);
        if(ImGui::TreeNode("scenes"))
        {
            ImGui::TreePop();
        }
        
        ImGui::SetNextTreeNodeOpen(true);
        if( ImGui::TreeNode("resources") )
        {
            ImGui::TreePop();    
        }

        ImGui::TreePop();
    }

    ImGui::End();
}