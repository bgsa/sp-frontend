#ifndef SP_PROPERTIES_FRAME_COMPONENT_CAMERA_HEADER
#define SP_PROPERTIES_FRAME_COMPONENT_CAMERA_HEADER

#include "SpectrumFronend.h"
#include "SpScene.h"
#include "SpGameObject.h"
#include "SpImGui.h"
#include "SpPropertiesFrameComponent.h"

namespace NAMESPACE_FRONTEND
{
	class SpPropertiesFrameComponentCamera
		: public SpPropertiesFrameComponent
	{
	public:

		API_INTERFACE inline void render(SpScene* scene, const sp_uint index) override
		{
			SpCamera* camera = scene->camerasManager()->get(index);
			Vec3 position = camera->position();
			Vec3 target = camera->target();
			Vec3 up = camera->up();
			Vec3 right = camera->right();
			Vec3 forward = camera->forward();

			ImGui::Text("Position");
			ImGui::PushItemWidth(80.0f);
			ImGui::InputFloat("X##position", (sp_float*)&position.x, 0.0f, 0.0f, 4);
			ImGui::SameLine();
			ImGui::InputFloat("Y##position", (sp_float*)&position.y, 0.0f, 0.0f, 4);
			ImGui::SameLine();
			ImGui::InputFloat("Z##position", (sp_float*)&position.z, 0.0f, 0.0f, 4);
			ImGui::PopItemWidth();

			if (position != camera->position())
				camera->position(position);

			ImGui::Text("Target");
			ImGui::PushItemWidth(80.0f);
			ImGui::InputFloat("X##target", (sp_float*)&target.x, 0.0f, 0.0f, 4);
			ImGui::SameLine();
			ImGui::InputFloat("Y##target", (sp_float*)&target.y, 0.0f, 0.0f, 4);
			ImGui::SameLine();
			ImGui::InputFloat("Z##target", (sp_float*)&target.z, 0.0f, 0.0f, 4);
			ImGui::PopItemWidth();

			if (target != camera->target())
				camera->target(target);

			ImGui::Text("Up");
			ImGui::PushItemWidth(80.0f);
			ImGui::InputFloat("X##up", (sp_float*)&up.x, 0.0f, 0.0f, 4, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			ImGui::InputFloat("Y##up", (sp_float*)&up.y, 0.0f, 0.0f, 4, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			ImGui::InputFloat("Z##up", (sp_float*)&up.z, 0.0f, 0.0f, 4, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			ImGui::Text("Right");
			ImGui::PushItemWidth(80.0f);
			ImGui::InputFloat("X##right", (sp_float*)&right.x, 0.0f, 0.0f, 4, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			ImGui::InputFloat("Y##right", (sp_float*)&right.y, 0.0f, 0.0f, 4, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			ImGui::InputFloat("Z##right", (sp_float*)&right.z, 0.0f, 0.0f, 4, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			ImGui::Text("Right");
			ImGui::PushItemWidth(80.0f);
			ImGui::InputFloat("X##forward", (sp_float*)&forward.x, 0.0f, 0.0f, 4, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			ImGui::InputFloat("Y##forward", (sp_float*)&forward.y, 0.0f, 0.0f, 4, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			ImGui::InputFloat("Z##forward", (sp_float*)&forward.z, 0.0f, 0.0f, 4, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			sp_float fieldOfView = camera->fieldOfView();
			ImGui::Text("Field of View:"); ImGui::SameLine();
			ImGui::SliderFloat("##", &fieldOfView, SP_CAMERA_MIN_FIELD_OF_VIEW, SP_CAMERA_MAX_FIELD_OF_VIEW, "%.2f", 1.0f);
			camera->fieldOfView(fieldOfView);

			sp_float velocity = camera->velocity();
			ImGui::Text("Velocity:"); ImGui::SameLine();
			ImGui::InputFloat("##", &velocity, 0.0f, 0.0f, 3);
			camera->velocity(velocity);
		}

	};
}

#endif // SP_PROPERTIES_FRAME_COMPONENT_CAMERA_HEADER