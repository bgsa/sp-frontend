#include "SpUIFonts.h"

namespace NAMESPACE_FRONTEND
{
	SpUIFonts* SpUIFontsInstance = nullptr;

	void SpUIFonts::init()
	{
		if (SpUIFontsInstance == nullptr)
		{
			SpUIFontsInstance = sp_mem_new(SpUIFonts);

			ImGuiIO& io = ImGui::GetIO();
			//SpUIFontsInstance->fontDefault = io.Fonts->AddFontDefault();
			SpUIFontsInstance->fontRobotoRegular = io.Fonts->AddFontFromFileTTF("resources/fonts/roboto/Roboto-Regular.ttf", 18.0f);
			SpUIFontsInstance->fontDefault = SpUIFontsInstance->fontRobotoRegular;
		}
	}

	void SpUIFonts::release()
	{
		if (SpUIFontsInstance != nullptr)
		{
			sp_mem_delete(SpUIFontsInstance, SpUIFonts);
			SpUIFontsInstance = nullptr;
		}
	}

}