#pragma once
#include "../global.h"
#include <StringUtils.h>

namespace ul {
	using assetnodeid_t = unsigned;
	class AssetLocation;
	class AssetsManager;

	template<typename _T>class Asset {
	public:
		/*
			TOW: did type deduction works for universal refs with types as class typename ?
		*/
		Asset(ulString name, _T&& data) : m_Name{ std::move(name) }, m_Data{ std::forward<_T>(data) } {}

		virtual _T get() = 0;

	protected:
		ulString m_Name;
		_T m_Data;
	};

}