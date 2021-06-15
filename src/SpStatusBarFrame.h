#ifndef SP_STATUS_BAR_FRAME_HEADER
#define SP_STATUS_BAR_FRAME_HEADER

#include "SpIFrameComponent.h"

namespace NAMESPACE_FRONTEND
{
	class SpStatusBarFrame
		: public SpIFrameComponent
	{
	private:
		
	public:

		API_INTERFACE inline void init()
		{
		}

		API_INTERFACE void begin();

		API_INTERFACE void beginChild();

		API_INTERFACE void render() override
		{
		}

		API_INTERFACE inline void end()
		{
			ImGui::End();

			ImGui::PopStyleVar();
		}

		API_INTERFACE inline void endChild()
		{
			ImGui::EndChild();

			ImGui::PopStyleVar();
		}

	};
}

#endif // SP_STATUS_BAR_FRAME_HEADER