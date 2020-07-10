#include "AssetsManager.h"
#include <iostream>

namespace ul {

	inline AssetsManager::id_t AssetsManager::addFolder(const path_t& path) {
		return addFolder(path, alias_t());
	}

	inline AssetsManager::id_t AssetsManager::addFolder(const path_t& path, const alias_t& alias) {
		m_Folders.push_back(std::make_pair(alias, path));
		return m_Folders.size() - 1;
	}

	inline void AssetsManager::addFolderRecurse(const path_t& path, idlist_t* ids) {
		id_t cId = addFolder(path);
		if (ids) ids->push_back(std::make_pair(path, cId));
		for (const auto& it : std::filesystem::recursive_directory_iterator(m_Root / path)) {
			if (it.is_directory()) {
				cId = addFolder(it);
				if (ids) ids->push_back(std::make_pair(it, cId));
			}
		}
	}

	inline AssetsManager::path_t AssetsManager::getPath(const path_t& file) {
		const auto& filename = file.filename(); //filename() parse at each call
		for (const auto& folder : m_Folders) {
			const path_t& tmpPath = findInFolder(m_Root / folder.second, filename);
			if (!tmpPath.empty()) return tmpPath;
		}
		return path_t();
	}

	inline AssetsManager::path_t AssetsManager::getPath(const path_t& file, const alias_t& falias) {
		const auto& filename = file.filename();
		if (falias.empty()) return getPath(file);
		for (const auto& folder : m_Folders) {
			if (folder.first == falias)
				return findInFolder(m_Root / folder.second, filename);
		}
		return path_t();
	}

	inline AssetsManager::path_t AssetsManager::getPath(const path_t& file, id_t fid) {
		return findInFolder(m_Root / m_Folders[fid].second, file.filename());
	}

	inline AssetsManager::path_t AssetsManager::getPath(id_t fid) {
		return m_Root / m_Folders[fid].second;
	}

	inline AssetsManager::path_t AssetsManager::findInFolder(const path_t& folder, const path_t::string_type& filename) {
		static path_t empt{};
		for (const auto& it : std::filesystem::directory_iterator(folder)) {
			if (it.path().filename() == filename)
				return it.path();
		}
		return empt;
	}

	template<typename _T>inline _T AssetsManager::get(const path_t& file) {
		const auto& filename = file.filename(); //filename() parse at each call
		for (const auto& folder : m_Folders) {
			const path_t& tmpPath = findInFolder(m_Root / folder.second, filename);
			if (!tmpPath.empty()) return AssetLoader<_T>(tmpPath);
		}
		return AssetLoader<_T>(path_t());
	}

	template<typename _T>
	inline _T AssetsManager::get(const path_t& file, const alias_t& falias) {
		const auto& filename = file.filename();
		if (falias.empty()) return get<_T>(file);
		for (const auto& folder : m_Folders) {
			if (folder.first == falias)
				return AssetLoader<_T>(findInFolder(m_Root / folder.second, filename));
		}
		return AssetLoader<_T>(path_t());
	}

	template<typename _T>
	inline _T AssetsManager::get(const path_t& file, id_t fid) {
		return AssetLoader<_T>(findInFolder(m_Root / m_Folders[fid].second, file.filename()));

	}

}
	//UL Specific
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