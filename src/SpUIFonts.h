#ifndef SP_UI_FONTS_HEADER
#define SP_UI_FONTS_HEADER

#include "SpectrumFronend.h"
#include "SpImGui.h"

namespace NAMESPACE_FRONTEND
{
	class SpUIFonts
	{
	public:
		ImFont* fontDefault;
		ImFont* fontRobotoRegular;

		API_INTERFACE static void init();
		API_INTERFACE static void release();

	};

	extern SpUIFonts* SpUIFontsInstance;
}

#endif // SP_UI_FONTS_HEADER