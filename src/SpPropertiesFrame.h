#ifndef SP_PROPERTIES_FRAME_HEADER
#define SP_PROPERTIES_FRAME_HEADER

#include "SpIFrameComponent.h"
#include "SpPhysicSimulator.h"
#include "SpVector.h"
#include "SpMap.h"
#include "SpPropertyInfo.h"
#include "SpScene.h"
#include "SpPropertiesFrameComponent.h"

namespace NAMESPACE_FRONTEND
{
	class SpPropertiesFrame
		: public SpIFrameComponent
	{
	private:
		SpScene* _scene;
		SpPropertiesFrameComponent* _propertiesComponent;
		sp_uint _selectedIndex;

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

	public:

		API_INTERFACE inline SpPropertiesFrame()
		{
			_propertiesComponent = nullptr;
			_selectedIndex = SP_UINT_MAX;
		}

		API_INTERFACE void init()
		{
			_minWidth = 100.0f;

			show();
		}

		API_INTERFACE inline SpPropertiesFrameComponent* propertiesComponent() const
		{
			return _propertiesComponent;
		}

		API_INTERFACE inline sp_uint selectedIndex() const
		{
			return _selectedIndex;
		}

		API_INTERFACE inline void select(SpScene* scene, SpPropertiesFrameComponent* component, const sp_uint index)
		{
			_scene = scene;
			_propertiesComponent = component;
			_selectedIndex = index;
		}

		API_INTERFACE inline void deselect()
		{
			_scene = nullptr;
			_propertiesComponent = nullptr;
			_selectedIndex = SP_UINT_MAX;
		}

		API_INTERFACE void render() override;

	};
}

#endif // SP_PROPERTIES_FRAME_HEADER