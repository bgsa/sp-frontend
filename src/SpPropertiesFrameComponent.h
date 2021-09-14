#ifndef SP_PROPERTIES_FRAME_COMPONENT_HEADER
#define SP_PROPERTIES_FRAME_COMPONENT_HEADER

#include "SpFrame.h"

namespace NAMESPACE_FRONTEND
{
	class SpPropertiesFrameComponent
	{
	public:

		API_INTERFACE virtual void render(SpScene* scene, const sp_uint index, void* value) = 0;

		API_INTERFACE virtual void show()
		{
		}

		API_INTERFACE virtual void hide()
		{
		}

	};
}

#endif // SP_PROPERTIES_FRAME_COMPONENT_HEADER