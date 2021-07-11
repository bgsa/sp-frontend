#ifndef SP_PROPERTIES_FRAME_COMPONENT_GAME_OBJECT_HEADER
#define SP_PROPERTIES_FRAME_COMPONENT_GAME_OBJECT_HEADER

#include "SpectrumFronend.h"
#include "SpScene.h"
#include "SpGameObject.h"
#include "SpImGui.h"
#include "SpPropertiesFrameComponent.h"

namespace NAMESPACE_FRONTEND
{
	class SpPropertiesFrameComponentGameObject
		: public SpPropertiesFrameComponent
	{
	public:

		API_INTERFACE inline void render(SpScene* scene, const sp_uint index) override
		{
			SpGameObject* gameObject = scene->gameObject(index);
			sp_char* name = gameObject->name();
			ImGui::Text(name);

			SpTransform* transform = scene->transform(index);

			ImGui::SetNextTreeNodeOpen(true);
			if (ImGui::TreeNode("Transform"))
			{
				ImGui::Text("Position");
				ImGui::PushItemWidth(80.0f);
				ImGui::InputFloat("X##position", (sp_float*)&transform->position.x, 0.0f, 0.0f, 4);
				ImGui::SameLine();
				ImGui::InputFloat("Y##position", (sp_float*)&transform->position.y, 0.0f, 0.0f, 4);
				ImGui::SameLine();
				ImGui::InputFloat("Z##position", (sp_float*)&transform->position.z, 0.0f, 0.0f, 4);
				ImGui::PopItemWidth();

				Vec3 angles;
				eulerAnglesXYZ(transform->orientation, angles);
				sp_float angleX = degree(angles.x);
				sp_float angleY = degree(angles.y);
				sp_float angleZ = degree(angles.z);

				ImGui::Text("Orientation");
				ImGui::PushItemWidth(80.0f);
				ImGui::InputFloat("X##orientation", &angleX, 0.0f, 0.0f, 2);
				ImGui::SameLine();
				ImGui::InputFloat("Y##orientation", &angleY, 0.0f, 0.0f, 2);
				ImGui::SameLine();
				ImGui::InputFloat("Z##orientation", &angleZ, 0.0f, 0.0f, 2);
				ImGui::PopItemWidth();

				fromEulerAngles(radians(angleX), radians(angleY), radians(angleZ), transform->orientation);

				ImGui::Text("Scale");
				ImGui::PushItemWidth(80.0f);
				ImGui::InputFloat("X##scale", (sp_float*)&transform->scaleVector.x, 0.0f, 0.0f, 4);
				ImGui::SameLine();
				ImGui::InputFloat("Y##scale", (sp_float*)&transform->scaleVector.y, 0.0f, 0.0f, 4);
				ImGui::SameLine();
				ImGui::InputFloat("Z##scale", (sp_float*)&transform->scaleVector.z, 0.0f, 0.0f, 4);
				ImGui::PopItemWidth();

				ImGui::TreePop();
			}
		}

	};
}

#endif // SP_PROPERTIES_FRAME_COMPONENT_GAME_OBJECT_HEADER