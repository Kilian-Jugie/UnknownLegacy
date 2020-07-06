#pragma once
#include <type_traits>
#include <vector>
#include <filesystem>

namespace ul {

	template<typename _T>_T AssetLoader(const std::filesystem::path&) = delete;

	class AssetsManager {
	public:
		using path_t = std::filesystem::path;
		using id_t = uint_fast64_t;
		using idlist_t = std::vector<std::pair<path_t, id_t>>;
		using alias_t = std::string;

		AssetsManager(path_t root) : m_Root{ std::move(root) } {

		}

		const std::vector<std::pair<alias_t, path_t>>& folders() {
			return m_Folders;
		}

		id_t addFolder(const path_t& path);
		id_t addFolder(const path_t& path, const alias_t& alias);
		void addFolderRecurse(const path_t& path, idlist_t* ids = nullptr);
		template<typename _T>_T get(const path_t& file);
		template<typename _T>_T get(const path_t& file, const alias_t& falias);
		template<typename _T>_T get(const path_t& file, id_t fid);

		path_t getPath(const path_t& file);
		path_t getPath(const path_t& file, const alias_t& falias);
		path_t getPath(const path_t& file, id_t fid);

	private:
		path_t findInFolder(const path_t& folder, const path_t::string_type& filename);

		path_t m_Root;
		std::vector<std::pair<alias_t, path_t>> m_Folders;
	};

}

#include "AssetsManager.inl"