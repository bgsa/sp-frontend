#ifndef SP_PROPERTIES_FRAME_COMPONENT_FOLDER_HEADER
#define SP_PROPERTIES_FRAME_COMPONENT_FOLDER_HEADER

#include "SpectrumFronend.h"
#include "FileSystem.h"
#include "SpScene.h"
#include "SpImGui.h"
#include "SpPropertiesFrameComponent.h"

namespace NAMESPACE_FRONTEND
{
	class SpPropertiesFrameComponentFolder
		: public SpPropertiesFrameComponent
	{
	private:
		sp_char _name[SP_DIRECTORY_MAX_LENGTH];
		sp_uint _nameLength;

	public:

		API_INTERFACE inline void setName(const sp_char* name, const sp_uint nameLength)
		{
			_nameLength = nameLength;
			std::memcpy(_name, name, nameLength);
		}

		API_INTERFACE inline void render(SpScene* scene, const sp_uint index, void* value) override
		{
			const sp_size indexOf = strLastIndexOf(_name, _nameLength, SP_DIRECTORY_SEPARATOR);

			sp_char name[SP_DIRECTORY_MAX_LENGTH];
			sp_size nameLength = _nameLength - indexOf - 1;
			std::memcpy(name, &_name[indexOf + 1], nameLength);
			name[nameLength] = END_OF_STRING;

			sp_char tempName[SP_DIRECTORY_MAX_LENGTH];
			std::memcpy(tempName, name, nameLength);
			tempName[nameLength] = END_OF_STRING;

			ImGui::Text("Name: "); ImGui::SameLine();
			ImGui::InputText("##prop-folder-name", tempName, SP_DIRECTORY_MAX_LENGTH);

			if (std::strcmp(name, tempName) != 0)
			{
				const sp_size tempNameLength = std::strlen(tempName);

				if (tempNameLength > 0)
				{
					sp_char newFolderName[SP_DIRECTORY_MAX_LENGTH];
					std::memcpy(newFolderName, _name, indexOf + 1);
					std::memcpy(&newFolderName[indexOf + 1], tempName, tempNameLength);

					const sp_size newFolderNameLength = indexOf + tempNameLength + 1;
					newFolderName[newFolderNameLength] = END_OF_STRING;

					if (directoryRename(_name, newFolderName))
					{
						std::memcpy(_name, newFolderName, newFolderNameLength);
						_name[newFolderNameLength] = END_OF_STRING;
						_nameLength = newFolderNameLength;
					}
				}
			}
		}

	};
}

#endif // SP_PROPERTIES_FRAME_COMPONENT_FOLDER_HEADER