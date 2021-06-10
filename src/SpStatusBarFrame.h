#ifndef SP_STATUS_BAR_FRAME_HEADER
#define SP_STATUS_BAR_FRAME_HEADER

#include "SpFrame.h"
#include "Timer.h"

namespace NAMESPACE_FRONTEND
{
	class SpStatusBarFrame
		: public SpFrame
	{
	private:
		Timer localTimer;

	public:

		API_INTERFACE void init(SpWindow* window) override
		{
			SpFrame::init(window);
			show();
			localTimer.start();
		}

		API_INTERFACE void preRender() override
		{
		}

		API_INTERFACE void render() override
		{
		}

		API_INTERFACE void renderGUI() override;

		API_INTERFACE inline const sp_char* toString() override
		{
			return "AboutFrame";
		}

	};
}

#endif // SP_STATUS_BAR_FRAME_HEADER