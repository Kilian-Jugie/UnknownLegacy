#pragma once
#include "AssetsManager.h"

namespace ul {

	template<typename _T, typename>
	inline AssetsManager& AssetsManager::addFolder(_T&& f, bool subs) {
		m_Folders.push_back(std::forward<path_t>(f));
		//We only check this now because we need f to be constructed as path if
		//it is passed as rvalue ref or non-path object
		if (!std::filesystem::is_directory(UL_LAST(m_Folders))) {
			this->m_Folders.erase(m_Folders.end() - 1);
			return *this;
		}
		if (subs) {
			for (auto& it : std::filesystem::recursive_directory_iterator(UL_LAST(m_Folders))) {
				if (std::filesystem::is_directory(it)) m_Folders.push_back(it);
			}
		}
		return *this;
	}

	template<typename _T, typename _pT, typename>
	inline _T& AssetsManager::get(_pT&& filename) {
		path_t file{ std::forward<path_t>(filename) };
		for (auto& f : m_Folders) {
			for (auto& it : std::filesystem::directory_iterator(f)) {
				if (it.path().filename().compare(file) == 0) return AssetLoader<_T>(it);
			}
		}
		return AssetLoader<_T>(std::filesystem::path());
	}

	template<typename _T, typename _p1T, typename _p2T, typename, typename>
	inline _T& AssetsManager::get(_p1T&& filename, _p2T&& folder) {
		path_t file{ std::forward<path_t>(filename) };
		for (auto& it : std::filesystem::directory_iterator(f)) {
			if (it.path().filename().compare(std::forward<path_t>(filename))) return AssetLoader<_T>(it);
		}
		return AssetLoader<_T>(std::filesystem::path());
	}

}