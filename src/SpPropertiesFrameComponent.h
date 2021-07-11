#ifndef SP_PROPERTIES_FRAME_COMPONENT_HEADER
#define SP_PROPERTIES_FRAME_COMPONENT_HEADER

#include "SpFrame.h"

namespace NAMESPACE_FRONTEND
{
	class SpPropertiesFrameComponent
	{
	public:

		API_INTERFACE virtual void render(SpScene* scene, const sp_uint index) = 0;

	};
}

#endif // SP_PROPERTIES_FRAME_COMPONENT_HEADER