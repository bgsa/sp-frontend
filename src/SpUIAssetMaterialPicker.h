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
		sp_uint _materialsLength;
		SpUIThumbNail* _thumbNails;

		inline void releaseThumbNails()
		{
			sp_mem_release(_thumbNails);
			_thumbNails = nullptr;
		}

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
		API_INTERFACE void init();

		/// <summary>
		/// Render the color picker
		/// </summary>
		/// <returns></returns>
		API_INTERFACE void render() override;

		/// <summary>
		/// Release all allocated resources
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void dispose() override
		{
			if (_thumbNails != nullptr)
				releaseThumbNails();
		}

		~SpUIAssetMaterialPicker()
		{
			dispose();
		}

	};
}

#endif // SP_UI_ASSET_MATERIAL_PICKER_HEADER