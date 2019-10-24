#include "AssetsManager.h"
#include "UlFilesystem.h"
#include "AssetLocation.h"
#include "../utils/ErrorManager.h"
#include <sstream>

ul::AssetsManager::AssetsManager(const std::filesystem::path& assetsFolder, ErrorManager& errorManager)
	: m_Path{ std::move(assetsFolder) }, m_ErrorManager{ errorManager } {
	createFolderIfNotExists(m_Path);
}

const std::filesystem::path& ul::AssetsManager::getAssetsPath() const noexcept {
	return m_Path;
}

size_t ul::AssetsManager::addLocation(AssetLocation& loc){
	for (std::vector<AssetLocation>::const_iterator l(m_Locations.cbegin()); l<m_Locations.cend(); ++l) {
		if (l->getName() == loc.getName()) {
			osstream_t os;
			os << UL_FS("The location '") << loc.getName() << UL_FS("' is already registered as an asset location ! This should be changed !\n");
			m_ErrorManager.copyError({ ErrorLevel::WARNING, os.str() });
			/*
				Modularity security lack ?
				What happend if we use a non random access iterator on this ?
			*/
			return l - m_Locations.begin();
		}
	}
	m_Locations.push_back(loc);
	loc = m_Locations[m_Locations.size() - 1];
	return m_Locations.size() - 1;
}

namespace ul {

	AssetLocation& AssetsManager::getLocation(size_t id) {
		/*
			This error must be fatal because use of invalid id for a location
			is like trying to get things in a non existing place.
		*/
		if (id<0 || id > m_Locations.size()) {
			osstream_t os;
			os << UL_FS("The location id ") << id << UL_FS(" is invalid !\n");
			m_ErrorManager.copyError({ ErrorLevel::FATAL, os.str() });
			__assume(0);
		}
		return m_Locations[id];
	}

	AssetLocation& AssetsManager::getLocation(string_t name, bool addIfNotExist) {
		for (std::vector<AssetLocation>::iterator it(m_Locations.begin()); it != m_Locations.end(); ++it) {
			if (it->getName() == name) return (*it);
		}
		if (addIfNotExist) {
			AssetLocation loc{ name, this->getAssetsPath() };
			return this->getLocation(this->addLocation(loc));
		}
		osstream_t os;
		os << UL_FS("The location with name '") << name << UL_FS("' does not exists !\n");
		m_ErrorManager.copyError({ ErrorLevel::FATAL, os.str() });
		__assume(0);
	}

}
