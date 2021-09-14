#ifndef SP_UI_ASSET_MATERIAL_PICKER_HEADER
#define SP_UI_ASSET_MATERIAL_PICKER_HEADER

#include "SpectrumFronend.h"
#include "SpIFrameComponent.h"
#include "SpColorRGBA.h"
#include "SpCloseButtonUIWindowBehaviour.h"
#include "SpUIThumbNail.h"
#include "SpAssetMaterial.h"

namespace NAMESPACE_FRONTEND
{
	class SpUIAssetMaterialPicker
		: public SpIFrameComponent
	{
	private:
		SpCloseButtonUIWindowBehaviour closeButton;

	public:

		void (*onSelect)(SpAssetMaterial*);

		/// <summary>
		/// Default constructor
		/// </summary>
		/// <returns></returns>
		API_INTERFACE SpUIAssetMaterialPicker();
		
		/// <summary>
		/// Initialize the material picker
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void init()
		{

		}

		/// <summary>
		/// Render the color picker
		/// </summary>
		/// <returns></returns>
		API_INTERFACE void render() override;

	};
}

#endif // SP_UI_ASSET_MATERIAL_PICKER_HEADER