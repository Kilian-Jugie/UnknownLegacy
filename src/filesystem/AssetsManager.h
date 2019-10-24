#pragma once
#include "../global.h"
#include "Asset.h"
#include <string>
#include <filesystem>

namespace ul {

	class AssetsManager {
	public:
		using string_t = std::basic_string<ulfchar_t>;
		using osstream_t = std::basic_ostringstream<ulfchar_t>;

		AssetsManager(const std::filesystem::path& assetsFolder, ErrorManager& errorManager);

		inline const std::filesystem::path& getAssetsPath() const noexcept {
			return m_Path;
		}

		size_t addLocation(AssetLocation& loc);

		AssetLocation& getLocation(size_t id);
		AssetLocation& getLocation(string_t name, bool addIfNotExist=false);

	private:
		ErrorManager& m_ErrorManager;
		std::filesystem::path m_Path;
		std::vector<AssetLocation> m_Locations;
	};
}