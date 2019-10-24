#include "UlFilesystem.h"

namespace ul {
	void createFolderIfNotExists(const std::filesystem::path& p) {
		if (!std::filesystem::exists(p))
			std::filesystem::create_directories(p);
	}
}