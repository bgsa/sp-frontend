#ifndef SP_PROPERTIES_FRAME_HEADER
#define SP_PROPERTIES_FRAME_HEADER

#include "SpIFrameComponent.h"
#include "SpPhysicSimulator.h"
#include "SpVector.h"
#include "SpMap.h"
#include "SpPropertyInfo.h"
#include "SpScene.h"

#include "SpCameraPropertiesFrame.h"

namespace NAMESPACE_FRONTEND
{
	class SpPropertiesFrame
		: public SpIFrameComponent
	{
	private:
		SpScene* _scene;
		sp_uint _selectedGameObject;

		inline void renderGameObjectProperties()
		{
			SpGameObject* gameObject = _scene->gameObject(_selectedGameObject);
			sp_char* name = gameObject->name();
			ImGui::Text(name);

			SpTransform* transform = _scene->transform(_selectedGameObject);

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
				ImGui::InputFloat("X##orientation", &angleX,  0.0f, 0.0f, 2);
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

			switch (gameObject->type())
			{
			case SP_GAME_OBJECT_TYPE_CAMERA:
				SpCameraPropertiesFrame cameraProperties;
				cameraProperties.renderProperties(_scene, _selectedGameObject);
				break;

			default:
				break;
			}
		}

		inline void renderVec3Property(const sp_char* name,  const Vec3& value)
		{
			sp_char* text = ALLOC_ARRAY(sp_char, 20);

			ImGui::Text(name);
			ImGui::NextColumn();
			ImGui::NextColumn();

			ImGui::Text("x:");
			ImGui::NextColumn();

			SpString::convert(value.x, text);
			ImGui::Text(text);
			ImGui::NextColumn();

			ImGui::Text("y:");
			ImGui::NextColumn();

			SpString::convert(value.y, text);
			ImGui::Text(text);
			ImGui::NextColumn();

			ImGui::Text("z:");
			ImGui::NextColumn();

			SpString::convert(value.z, text);
			ImGui::Text(text);
			ImGui::NextColumn();

			ALLOC_RELEASE(text);
		}

		inline void renderQuatProperty(const sp_char* name, const Quat& q)
		{
			sp_char* text = ALLOC_ARRAY(sp_char, 20);

			ImGui::Text(name);
			ImGui::NextColumn();
			ImGui::NextColumn();

			ImGui::Text("w:");
			ImGui::NextColumn();

			SpString::convert(q.w, text);
			ImGui::Text(text);
			ImGui::NextColumn();

			ImGui::Text("x:");
			ImGui::NextColumn();

			SpString::convert(q.x, text);
			ImGui::Text(text);
			ImGui::NextColumn();

			ImGui::Text("y:");
			ImGui::NextColumn();

			SpString::convert(q.y, text);
			ImGui::Text(text);
			ImGui::NextColumn();

			ImGui::Text("z:");
			ImGui::NextColumn();

			SpString::convert(q.z, text);
			ImGui::Text(text);
			ImGui::NextColumn();

			ALLOC_RELEASE(text);
		}

		inline void renderProperty(const sp_char* name, const sp_float value)
		{
			sp_char* text = ALLOC_ARRAY(sp_char, 20);
			SpString::convert(value, text);

			ImGui::Text(name);
			ImGui::NextColumn();

			ImGui::Text(text);
			ImGui::NextColumn();

			ALLOC_RELEASE(text);
		}

		inline void renderProperty(const sp_char* name, const sp_bool value)
		{
			ImGui::Text(name);
			ImGui::NextColumn();

			if (value)
				ImGui::Text("True");
			else
				ImGui::Text("False");

			ImGui::NextColumn();
		}

		inline void renderPhysicProperties()
		{
			const SpRigidBody3D* rigidBody = SpWorldManagerInstance->current()->rigidBody3D(_selectedGameObject);

			renderProperty("Static", rigidBody->isStatic());

			renderVec3Property("Posision", rigidBody->currentState.position());
			renderVec3Property("Velocity", rigidBody->currentState.velocity());
			renderVec3Property("Acceleration", rigidBody->currentState.acceleration());

			renderQuatProperty("Orientation", rigidBody->currentState.orientation());
			renderVec3Property("Angular Velocity", rigidBody->currentState.angularVelocity());
			renderVec3Property("Torque", rigidBody->currentState.torque());

			renderProperty("Mass", ONE_FLOAT / rigidBody->massInverse());
			renderProperty("Restitution", rigidBody->coeficientOfRestitution());
			renderProperty("Damping", rigidBody->damping());
			renderProperty("Friction", rigidBody->coeficientOfFriction());
		}

	public:

		API_INTERFACE void init()
		{
			_minWidth = 100.0f;

			show();

			_selectedGameObject = SP_UINT_MAX;
		}

		API_INTERFACE inline sp_uint selectedGameObject()
		{
			return _selectedGameObject;
		}

		API_INTERFACE inline void selectedGameObject(SpScene* scene, const sp_uint objectIndex)
		{
			_scene = scene;
			_selectedGameObject = objectIndex;
		}

		API_INTERFACE void render() override;

	};
}

#endif // SP_PROPERTIES_FRAME_HEADER