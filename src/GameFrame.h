#ifndef MAIN_WINDOW_HEADER
#define MAIN_WINDOW_HEADER

#define GLEW_STATIC

#include "SpectrumFronend.h"
#include "SpFrame.h"
#include "RenderData.h"
#include "Framebuffer.h"
#include "RendererSettings.h"
#include "OpenGLTexture.h"
#include "IRendererManager.h"
#include "GridSystem.h"
#include "SpWindow.h"
#include "DefaultRendererManager.h"
#include "EditorViewer.h"
#include "Rock.h"
#include "RockRenderer.h"

namespace NAMESPACE_FRONTEND
{
	class GameFrame 
		: public SpFrame
	{
	private:
		OpenGLTexture* texture = NULL;
		IRendererManager* renderer;
		EditorViewer viewer;

		GridSystem* gridSystem = NULL;
		Rock* rock = NULL;
		RockRenderer* rockRenderer = NULL;

	public:

		API_INTERFACE void init(SpWindow* window) override;

		API_INTERFACE void update(sp_longlong elapsedTime);

		API_INTERFACE void preRender() override;

		API_INTERFACE void render() override;

		API_INTERFACE void renderGUI() override;

		API_INTERFACE void postRender() override;

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