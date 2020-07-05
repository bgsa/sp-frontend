#ifndef PROPERTIES_FRAME_HEADER
#define PROPERTIES_FRAME_HEADER

#include "SpFrame.h"
#include "SpPhysicSimulator.h"
#include "SpVector.h"
#include "SpMap.h"
#include "SpPropertyInfo.h"

namespace NAMESPACE_FRONTEND
{
	class PropertiesFrame
		: public SpFrame
	{
	private:
		sp_uint _selectedObject;

		inline void renderPhysicProperties()
		{
			const SpPhysicProperties* physicProperties = SpPhysicSimulator::instance()->physicProperties(_selectedObject);

			ImGui::Text("Position");
			ImGui::NextColumn();

			sp_char* text = ALLOC_ARRAY(sp_char, 20);
			SpString::convert(physicProperties->position().y, text);
			ImGui::Text(text);
			ALLOC_RELEASE(text);
			
			ImGui::NextColumn();
		}

	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			resize(300, 600);
			show();

			_selectedObject = SP_UINT_MAX;
		}

		API_INTERFACE inline sp_uint selectedObject()
		{
			return _selectedObject;
		}

		API_INTERFACE inline void selectedObject(sp_uint globalIndex)
		{
			_selectedObject = globalIndex;
		}

		API_INTERFACE void preRender() override
		{
		}

		API_INTERFACE void render() override
		{
		}

		API_INTERFACE void renderGUI() override
		{
			if (!isVisible())
				return;

			if (_selectedObject == SP_UINT_MAX)
				return;

			resize(width(), height() - 25);

			if (ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoCollapse))
			{
				loadState();

				ImGui::SetWindowSize(ImVec2((sp_float)width(), (sp_float)height()));
				ImGui::SetWindowPos(ImVec2((sp_float)(window()->state()->width - width()), 25.0f));

				ImGui::Columns(2, "Bar");

				renderPhysicProperties();

				ImGui::End();
			}
		}

		API_INTERFACE void postRender() override
		{
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "PropertiesFrame";
		}

		~PropertiesFrame()
		{
		}

	};
}

#endif // PROPERTIES_FRAME_HEADER