#pragma once
#include "../global.h"
#include <type_traits>
#include <vector>
#include <filesystem>

namespace ul {
	template<typename _T>_T AssetLoader(std::filesystem::path&) = delete;
	

	template<>std::wstring AssetLoader(std::filesystem::path& p) {
		return L"";
	}

	class AssetsManager {
		using path_t = std::filesystem::path;

		std::vector<path_t> m_Folders;

	public:
		template<typename _T,
			typename = std::enable_if_t<std::is_constructible_v<path_t, std::decay_t<_T>>>>
			AssetsManager& addFolder(_T&& f, bool subs);

		template<typename _T, typename _pT,
			typename = std::enable_if_t<std::is_constructible_v<path_t, std::decay_t<_pT>>>>
			_T & get(_pT&& filename);

		template<typename _T, typename _p1T, typename _p2T,
			typename = std::enable_if_t<std::is_constructible_v<path_t, std::decay_t<_p1T>>>,
			typename = std::enable_if_t<std::is_constructible_v<path_t, std::decay_t<_p2T>>>>
			_T & get(_p1T&& filename, _p2T&& folder);


	};

}

#include "AssetsManager.inl"