#include "AssetLocation.h"
#include "AssetsManager.h"
#include "UlFilesystem.h"
#include "../global.h"

namespace ul {
	const std::filesystem::path& AssetLocation::getPath() const noexcept {
		return m_Path;
	}

	const AssetLocation::string_t& AssetLocation::getName() const noexcept {
		return m_Name;
	}

	void AssetLocation::_base() {
		//m_Manager.addLocation(*this);
		
		createFolderIfNotExists(m_Path);
	}
}