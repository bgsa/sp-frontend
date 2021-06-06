#ifndef NEW_SCENE_FRAME_HEADER
#define NEW_SCENE_FRAME_HEADER

#include "SpFrame.h"

namespace NAMESPACE_FRONTEND
{
	class NewSceneFrame
		: public SpFrame
	{
	private:
		sp_char name[100];
		SpVectorItem<SpFrame*>* frameId;

	public:

		API_INTERFACE void init(SpWindow* window) 
		{
			SpFrame::init(window);
			resize(400, 140);
		}

		API_INTERFACE void show() noexcept override;
		API_INTERFACE void hide() noexcept override;

		API_INTERFACE void preRender() override
		{
		}

		API_INTERFACE void render() override
		{
		}

		API_INTERFACE void renderGUI() override;

		API_INTERFACE inline const sp_char* toString() override
		{
			return "NewSceneFrame";
		}

	};
}

#endif // NEW_SCENE_FRAME_HEADER