#pragma once
#include <filesystem>

namespace ul {
	void createFolderIfNotExists(const std::filesystem::path& p);
}