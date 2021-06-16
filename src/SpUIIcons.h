#ifndef SP_UI_ICONS_HEADER
#define SP_UI_ICONS_HEADER

#include "SpectrumFronend.h"
#include "OpenGLTexture.h"
#include "SpImGui.h"

namespace NAMESPACE_FRONTEND
{
	class SpUIIcons
	{
	private:

		void initialize()
		{
			icons1 = sp_mem_new(OpenGLTexture)("resources/images/icons-32x32.png");
		}

	public:
		OpenGLTexture* icons1;

		API_INTERFACE static void init();
		API_INTERFACE static void release();

		API_INTERFACE inline void dispose()
		{
			if (icons1 != nullptr)
			{
				sp_mem_delete(icons1, OpenGLTexture);
				icons1 = nullptr;
			}
		}

		API_INTERFACE inline void getUVCoordinates(const sp_int row, const sp_int column, ImVec2& uv1, ImVec2& uv2)
		{
			const sp_float iconSpacing = 5.0f;
			const sp_float iconWidth = 32.0f + iconSpacing;

			const sp_float imageWidth = (sp_float)icons1->size().width;
			const sp_float imageHeight = (sp_float)icons1->size().height;

			uv1 = ImVec2(((column-1) * iconWidth) / imageWidth, ((row -1) * iconWidth) / imageHeight);
			uv2 = ImVec2((column * iconWidth) / imageWidth, (row * iconWidth) / imageHeight);
		}

		API_INTERFACE inline void getUVCoordinatesError(ImVec2& uv1, ImVec2& uv2)
		{
			getUVCoordinates(6, 1, uv1, uv2);
		}

		API_INTERFACE inline void getUVCoordinatesWarning(ImVec2& uv1, ImVec2& uv2)
		{
			getUVCoordinates(5, 11, uv1, uv2);
		}

		API_INTERFACE inline void getUVCoordinatesValidation(ImVec2& uv1, ImVec2& uv2)
		{
			getUVCoordinatesWarning(uv1, uv2);
		}

		API_INTERFACE inline void getUVCoordinatesInformation(ImVec2& uv1, ImVec2& uv2)
		{
			getUVCoordinates(6, 6, uv1, uv2);
		}

		API_INTERFACE inline void getUVCoordinatesOk(ImVec2& uv1, ImVec2& uv2)
		{
			getUVCoordinates(5, 13, uv1, uv2);
		}

		API_INTERFACE inline void getUVCoordinatesNextFrame(ImVec2& uv1, ImVec2& uv2)
		{
			getUVCoordinates(17, 8, uv1, uv2);
		}

		API_INTERFACE inline void getUVCoordinatesPlay(ImVec2& uv1, ImVec2& uv2)
		{
			getUVCoordinates(6, 3, uv1, uv2);
		}
		API_INTERFACE inline void getUVCoordinatesStop(ImVec2& uv1, ImVec2& uv2)
		{
			getUVCoordinates(6, 4, uv1, uv2);
		}

		~SpUIIcons()
		{
			dispose();
		}

	};

	extern SpUIIcons* SpUIIconsInstance;
}

#endif // SP_UI_ICONS_HEADER