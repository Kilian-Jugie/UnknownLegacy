#pragma once
#include <string>
#include <filesystem>
#include "Asset.h"
#include "AssetsManager.h"

#include "../utils/debug.h"

namespace ul {
	class AssetLocation {
	public:
		using string_t = std::basic_string<ulfchar_t>;

		/*
			To investigate: what about the tmp path& value when path::append ? rvalue (+ref?) ? 
			nrvo/rvo ? (and what about std::move in rvo case ?)

			To think about: making another constructor with !is_constructible with readable error
			message and in a perfect case use SFINAE to avoid compiler errors ? (Perfect API !)
		*/
		template<typename _T, typename = std::enable_if_t<std::is_constructible_v<string_t, _T>>>
		AssetLocation( _T&& name, std::filesystem::path basePath)
			:  m_Name{ std::forward<_T>(name) }, m_Path{ std::move(basePath.append(m_Name)) } {
			_base();
		}

		template<typename _T, typename _V,
			typename = std::enable_if_t<std::is_constructible_v<string_t, _T>>,
			typename = std::enable_if_t<std::is_constructible_v<string_t, _V>>>
		AssetLocation(_T&& name, _V&& foldername, std::filesystem::path basePath)
			: m_Name{ std::forward<_T>(name) }, m_Path{ std::move(basePath.append(std::forward<_V>(foldername))) } {
			_base();
		}

		AssetLocation(const AssetLocation&) = default;
		AssetLocation& operator=(const AssetLocation&) = default;

		const std::filesystem::path& getPath() const noexcept;
		const string_t& getName() const noexcept;

	private:
		void _base();

		string_t m_Name;
		std::filesystem::path m_Path;
	};
}