#ifndef SP_PROPERTIES_FRAME_COMPONENT_GAME_OBJECT_HEADER
#define SP_PROPERTIES_FRAME_COMPONENT_GAME_OBJECT_HEADER

#include "SpectrumFronend.h"
#include "SpScene.h"
#include "SpGameObject.h"
#include "SpImGui.h"
#include "SpPropertiesFrameComponent.h"
#include "SpUIColorPicker.h"

namespace NAMESPACE_FRONTEND
{
	class SpPropertiesFrameComponentGameObject
		: public SpPropertiesFrameComponent
	{
	private:
		SpUIColorPicker colorPicker;

		inline void renderMaterialColor(SpMaterial* material)
		{
			ImGui::Text("Color: "); ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(material->color.red, material->color.green, material->color.blue, material->color.alpha));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(material->color.red, material->color.green, material->color.blue, material->color.alpha));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(material->color.red, material->color.green, material->color.blue, material->color.alpha));

			if (ImGui::Button("##material-color", ImVec2(50.0f, 20.0f)))
			{
				colorPicker.init();
				colorPicker.color(&material->color);
				colorPicker.show();
			}
			if (ImGui::IsItemHovered())
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			colorPicker.render();
		}
		
		inline void renderMaterialAmbient(SpMaterial* material)
		{
			ImGui::Text("Ambient: ");
			ImGui::PushItemWidth(80.0f);
			ImGui::Text("Red:"); ImGui::SameLine();
			ImGui::InputFloat("##material-ambient-red", &material->ambient.red, 0.0f, 0.0f, 3);
			ImGui::SameLine();
			ImGui::Text("Green:"); ImGui::SameLine();
			ImGui::InputFloat("##material-ambient-green", &material->ambient.green, 0.0f, 0.0f, 3);
			ImGui::SameLine();
			ImGui::Text("Blue:"); ImGui::SameLine();
			ImGui::InputFloat("##material-ambient-blue", &material->ambient.blue, 0.0f, 0.0f, 3);
			ImGui::PopItemWidth();

			if (material->ambient.red > 1.0f)
				material->ambient.red = 1.0f;
			else if (material->ambient.red < 0.0f)
				material->ambient.red = 0.0f;

			if (material->ambient.green > 1.0f)
				material->ambient.green = 1.0f;
			else if (material->ambient.green < 0.0f)
				material->ambient.green = 0.0f;

			if (material->ambient.blue > 1.0f)
				material->ambient.blue = 1.0f;
			else if (material->ambient.blue < 0.0f)
				material->ambient.blue = 0.0f;
		}

		inline void renderMaterialDiffuse(SpMaterial* material)
		{
			ImGui::Text("Diffuse: ");
			ImGui::PushItemWidth(80.0f);
			ImGui::Text("Red:"); ImGui::SameLine();
			ImGui::InputFloat("##material-diffuse-red", &material->diffuse.red, 0.0f, 0.0f, 3);
			ImGui::SameLine();
			ImGui::Text("Green:"); ImGui::SameLine();
			ImGui::InputFloat("##material-diffuse-green", &material->diffuse.green, 0.0f, 0.0f, 3);
			ImGui::SameLine();
			ImGui::Text("Blue:"); ImGui::SameLine();
			ImGui::InputFloat("##material-diffuse-blue", &material->diffuse.blue, 0.0f, 0.0f, 3);
			ImGui::PopItemWidth();

			if (material->diffuse.red > 1.0f)
				material->diffuse.red = 1.0f;
			else if (material->diffuse.red < 0.0f)
				material->diffuse.red = 0.0f;

			if (material->diffuse.green > 1.0f)
				material->diffuse.green = 1.0f;
			else if (material->diffuse.green < 0.0f)
				material->diffuse.green = 0.0f;

			if (material->diffuse.blue > 1.0f)
				material->diffuse.blue = 1.0f;
			else if (material->diffuse.blue < 0.0f)
				material->diffuse.blue = 0.0f;
		}

		inline void renderMaterialSpecular(SpMaterial* material)
		{
			ImGui::Text("Specular: ");
			ImGui::PushItemWidth(80.0f);
			ImGui::Text("Red:"); ImGui::SameLine();
			ImGui::InputFloat("##material-specular-red", &material->specular.red, 0.0f, 0.0f, 3);
			ImGui::SameLine();
			ImGui::Text("Green:"); ImGui::SameLine();
			ImGui::InputFloat("##material-specular-green", &material->specular.green, 0.0f, 0.0f, 3);
			ImGui::SameLine();
			ImGui::Text("Blue:"); ImGui::SameLine();
			ImGui::InputFloat("##material-specular-blue", &material->specular.blue, 0.0f, 0.0f, 3);
			ImGui::PopItemWidth();

			if (material->specular.red > 1.0f)
				material->specular.red = 1.0f;
			else if (material->specular.red < 0.0f)
				material->specular.red = 0.0f;

			if (material->specular.green > 1.0f)
				material->specular.green = 1.0f;
			else if (material->specular.green < 0.0f)
				material->specular.green = 0.0f;

			if (material->specular.blue > 1.0f)
				material->specular.blue = 1.0f;
			else if (material->specular.blue < 0.0f)
				material->specular.blue = 0.0f;
		}

	public:

		API_INTERFACE inline void render(SpScene* scene, const sp_uint index) override
		{
			colorPicker.init();

			SpGameObject* gameObject = scene->gameObject(index);
			sp_char* name = gameObject->name();
			ImGui::Text(name);

			SpTransform* transform = scene->transform(index);

			ImGui::SetNextTreeNodeOpen(true);
			if (ImGui::TreeNode("Transform"))
			{
				ImGui::Text("Position");
				ImGui::PushItemWidth(80.0f);
				ImGui::Text("x:"); ImGui::SameLine();
				ImGui::InputFloat("##position-x", (sp_float*)&transform->position.x, 0.0f, 0.0f, 4);
				ImGui::SameLine();
				ImGui::Text("y:"); ImGui::SameLine();
				ImGui::InputFloat("##position-y", (sp_float*)&transform->position.y, 0.0f, 0.0f, 4);
				ImGui::SameLine();
				ImGui::Text("z:"); ImGui::SameLine();
				ImGui::InputFloat("##position-z", (sp_float*)&transform->position.z, 0.0f, 0.0f, 4);
				ImGui::PopItemWidth();

				Vec3 angles;
				eulerAnglesXYZ(transform->orientation, angles);
				sp_float angleX = degree(angles.x);
				sp_float angleY = degree(angles.y);
				sp_float angleZ = degree(angles.z);

				ImGui::Text("Orientation");
				ImGui::PushItemWidth(80.0f);
				ImGui::Text("x:"); ImGui::SameLine();
				ImGui::InputFloat("##orientation-x", &angleX, 0.0f, 0.0f, 2);
				ImGui::SameLine();
				ImGui::Text("y:"); ImGui::SameLine();
				ImGui::InputFloat("##orientation-y", &angleY, 0.0f, 0.0f, 2);
				ImGui::SameLine();
				ImGui::Text("z:"); ImGui::SameLine();
				ImGui::InputFloat("##orientation-z", &angleZ, 0.0f, 0.0f, 2);
				ImGui::PopItemWidth();

				fromEulerAngles(radians(angleX), radians(angleY), radians(angleZ), transform->orientation);

				ImGui::Text("Scale");
				ImGui::PushItemWidth(80.0f);
				ImGui::Text("x:"); ImGui::SameLine();
				ImGui::InputFloat("##scale-x", (sp_float*)&transform->scaleVector.x, 0.0f, 0.0f, 4);
				ImGui::SameLine();
				ImGui::Text("y:"); ImGui::SameLine();
				ImGui::InputFloat("##scale-y", (sp_float*)&transform->scaleVector.y, 0.0f, 0.0f, 4);
				ImGui::SameLine();
				ImGui::Text("z:"); ImGui::SameLine();
				ImGui::InputFloat("##scale-z", (sp_float*)&transform->scaleVector.z, 0.0f, 0.0f, 4);
				ImGui::PopItemWidth();

				ImGui::TreePop();
			}

			if (gameObject->isRenderableObject())
			{
				const sp_size renderableObjectIndex = gameObject->renderableObjectIndex();
				SpMaterial* material = scene->renderableObjectManager()->material(renderableObjectIndex);

				ImGui::SetNextTreeNodeOpen(true);
				if (ImGui::TreeNode("Material"))
				{
					renderMaterialColor(material);
					renderMaterialAmbient(material);
					renderMaterialDiffuse(material);
					renderMaterialSpecular(material);

					ImGui::Text("Shinines Factor: "); ImGui::SameLine();
					ImGui::InputFloat("##material-shininess-factor", &material->shininessFactor, 0.0f, 0.0f, 2);

					ImGui::TreePop();
				}
			}
		}

	};
}

#endif // SP_PROPERTIES_FRAME_COMPONENT_GAME_OBJECT_HEADER