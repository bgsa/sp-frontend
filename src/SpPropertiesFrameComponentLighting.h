#ifndef SP_PROPERTIES_FRAME_COMPONENT_LIGHTING_HEADER
#define SP_PROPERTIES_FRAME_COMPONENT_LIGHTING_HEADER

#include "SpectrumFronend.h"
#include "SpScene.h"
#include "SpImGui.h"
#include "SpPropertiesFrameComponent.h"

namespace NAMESPACE_FRONTEND
{
	class SpPropertiesFrameComponentLighting
		: public SpPropertiesFrameComponent
	{
	private:

		inline void renderLightName(SpScene* scene, const sp_uint index)
		{
			sp_char* name = scene->lightingManager()->name(index);

			ImGui::Text("Name: "); ImGui::SameLine();
			ImGui::InputText("##prop-light-name", name, SP_LIGHT_NAME_MAX_LENGTH);
		}

		inline void renderLightType(SpLightSource* light)
		{
			sp_char selectedType[10];
			switch (light->type())
			{
			case SP_LIGHT_SOURCE_TYPE_AMBIENT:
				std::memcpy(selectedType, "Ambient\0", 8);
				break;
			case SP_LIGHT_SOURCE_TYPE_DIFFUSE:
				std::memcpy(selectedType, "Diffuse\0", 8);
				break;
			case SP_LIGHT_SOURCE_TYPE_SPECULAR:
				std::memcpy(selectedType, "Specular\0", 9);
				break;
			}

			ImGui::Text("Type: "); ImGui::SameLine();
			if (ImGui::BeginCombo("##prop-light-type", selectedType))
			{
				bool selected = light->type() == SP_LIGHT_SOURCE_TYPE_AMBIENT;
				if (ImGui::Selectable("Ambient", selected))
					light->type(SP_LIGHT_SOURCE_TYPE_AMBIENT);
				if (selected)
					ImGui::SetItemDefaultFocus();

				selected = light->type() == SP_LIGHT_SOURCE_TYPE_DIFFUSE;
				if (ImGui::Selectable("Diffuse", selected))
					light->type(SP_LIGHT_SOURCE_TYPE_DIFFUSE);
				if (selected)
					ImGui::SetItemDefaultFocus();

				selected = light->type() == SP_LIGHT_SOURCE_TYPE_SPECULAR;
				if (ImGui::Selectable("Specular", selected))
					light->type(SP_LIGHT_SOURCE_TYPE_SPECULAR);
				if (selected)
					ImGui::SetItemDefaultFocus();

				ImGui::EndCombo();
			}

		}

		inline void renderLightPosition(SpLightSource* light)
		{
			Vec3 position = light->position();

			ImGui::Text("Position");
			ImGui::PushItemWidth(80.0f);
			ImGui::Text("x:"); ImGui::SameLine();
			ImGui::InputFloat("##prop-light-pos-x", &position.x, 0.0f, 0.0f, 4);
			ImGui::SameLine();
			ImGui::Text("y:"); ImGui::SameLine();
			ImGui::InputFloat("##prop-light-pos-y", &position.y, 0.0f, 0.0f, 4);
			ImGui::SameLine();
			ImGui::Text("z:"); ImGui::SameLine();
			ImGui::InputFloat("##prop-light-pos-z", &position.z, 0.0f, 0.0f, 4);
			ImGui::PopItemWidth();

			Vec3 newPos = light->position();

			if (!isCloseEnough(position.x, newPos.x))
				newPos.x = position.x;
			if (!isCloseEnough(position.y, newPos.y))
				newPos.y = position.y;
			if (!isCloseEnough(position.z, newPos.z))
				newPos.z = position.z;

			light->position(newPos);
		}

		inline void renderLightDirection(SpLightSource* light)
		{
			Vec3 dir = light->direction();

			ImGui::Text("Direction");
			ImGui::PushItemWidth(80.0f);
			ImGui::Text("x:"); ImGui::SameLine();
			ImGui::InputFloat("##prop-light-dir-x", &dir.x, 0.0f, 0.0f, 4);
			ImGui::SameLine();
			ImGui::Text("y:"); ImGui::SameLine();
			ImGui::InputFloat("##prop-light-dir-y", &dir.y, 0.0f, 0.0f, 4);
			ImGui::SameLine();
			ImGui::Text("z:"); ImGui::SameLine();
			ImGui::InputFloat("##prop-light-dir-z", &dir.z, 0.0f, 0.0f, 4);
			ImGui::PopItemWidth();

			Vec3 newDir = light->direction();

			if (!isCloseEnough(dir.x, newDir.x))
				newDir.x = dir.x;
			if (!isCloseEnough(dir.y, newDir.y))
				newDir.y = dir.y;
			if (!isCloseEnough(dir.z, newDir.z))
				newDir.z = dir.z;

			light->direction(newDir);
		}

		inline void renderLightEnabled(SpLightSource* light)
		{
			sp_bool enabled = light->isEnabled();
			ImGui::Checkbox("Enabled", &enabled);
			light->lightSwitch(enabled);
		}

		inline void renderLightStatic(SpLightSource* light)
		{
			sp_bool isStatic = light->isStatic();
			ImGui::Checkbox("Static", &isStatic);
			light->staticLight(isStatic);
		}

		inline void renderLightIntensity(SpLightSource* light)
		{
			sp_float intensity = light->factor();

			ImGui::Text("Intensity:"); ImGui::SameLine();
			ImGui::InputFloat("##prop-light-intensity", &intensity, 0.0f, 0.0f, 4);

			if (!isCloseEnough(intensity, light->factor()))
				light->factor(intensity);
		}

	public:

		API_INTERFACE inline void render(SpScene* scene, const sp_uint index, void* value) override
		{
			SpLightSource* light = scene->lightingManager()->get(index);

			renderLightName(scene, index);
			renderLightType(light);
			renderLightEnabled(light);
			renderLightPosition(light);
			renderLightDirection(light);
			renderLightStatic(light);
			renderLightIntensity(light);
		}

	};
}

#endif // SP_PROPERTIES_FRAME_COMPONENT_LIGHTING_HEADER