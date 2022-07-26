#include "AssetsManager.h"
#include <fstream>
#include <sstream>

namespace ul {
	template<>std::wstring AssetLoader(const std::filesystem::path& p) {
		std::wifstream in{ p };
		std::wstringstream buf;
		buf << in.rdbuf();
		in.close();
		return buf.str();
	}

	template<>std::string AssetLoader(const std::filesystem::path& p) {
		std::ifstream in{ p };
		std::stringstream buf;
		buf << in.rdbuf();
		in.close();
		return buf.str();
	}
}