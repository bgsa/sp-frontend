#ifndef SP_GAME_SETTINGS_HEADER
#define SP_GAME_SETTINGS_HEADER

#define GLEW_STATIC

#include "SpectrumFronend.h"
#include "SpPhysicSettings.h"

namespace NAMESPACE_FRONTEND
{
	class SpGameSettings
	{
	private:
		
		SpGameSettings()
		{
		}
		
	public:

		API_INTERFACE inline static SpGameSettings* instance()
		{
			static SpGameSettings* _settings = sp_mem_new(SpGameSettings)();
			return _settings;
		}

	};
}

#endif // SP_GAME_SETTINGS_HEADER