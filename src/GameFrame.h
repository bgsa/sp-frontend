#ifndef MAIN_WINDOW_HEADER
#define MAIN_WINDOW_HEADER

#define GLEW_STATIC

#include "SpectrumFronend.h"
#include "SpFrame.h"
#include "RenderData.h"
#include "Framebuffer.h"
#include "RendererSettings.h"
#include "OpenGLTexture.h"
#include "GridSystem.h"
#include "SpWindow.h"
#include "OpenGLRendererManager.h"
#include "EditorViewer.h"

#include "RendererList.h"
#include "DOP18ListRenderer.h"
#include "Rock.h"
#include "RockRenderer.h"
#include "RockList.h"

namespace NAMESPACE_FRONTEND
{
	class GameFrame 
		: public SpFrame
		, public SpKeyboardEventListener
	{
	private:
		OpenGLTexture* texture = nullptr;
		OpenGLRendererManager* renderer;
		EditorViewer viewer;

		GridSystem* gridSystem = nullptr;
		Rock* rock1 = nullptr;
		Rock* rock2 = nullptr;
		RockRenderer* rockRenderer = nullptr;
		RockList* rockList = nullptr;

		sp_float gameVelocity = 0.1f;

	public:

		API_INTERFACE void init(SpWindow* window) override;

		API_INTERFACE void update(sp_longlong elapsedTime);

		API_INTERFACE void preRender() override;

		API_INTERFACE void render() override;

		API_INTERFACE void renderGUI() override;

		API_INTERFACE void postRender() override;

		API_INTERFACE void onKeyboardEvent(SpKeyboardEvent* evt) override;

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Main Frame";
		}

		API_INTERFACE void dispose() override;

		~GameFrame()
		{
			dispose();
		}

	};
}

#endif // MAIN_WINDOW_HEADER