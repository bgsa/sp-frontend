#ifndef SP_CAMERA_PROPERTIES_FRAME_HEADER
#define SP_CAMERA_PROPERTIES_FRAME_HEADER

#include "SpectrumFronend.h"
#include "SpScene.h"
#include "SpGameObject.h"
#include "SpImGui.h"

namespace NAMESPACE_FRONTEND
{
	class SpCameraPropertiesFrame
	{
	public:

		API_INTERFACE inline void renderProperties(SpScene* scene, sp_uint index)
		{
			SpGameObject* obj = scene->gameObject(index);
			SpCamera* camera = scene->cameras()->get(obj->managerIndex());

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

#endif // SP_CAMERA_PROPERTIES_FRAME_HEADER