#pragma once
#include "kPreprocessor.h"
#include <string>

namespace ul {
	class UnknownLegacy;
	class AssetLocation;
	class AssetsManager;
	class Error;
	class ErrorManager;
	template<typename _ChT>class Stream;
	template<typename _ChT>class OutSecureStream;
	template<typename _ChT>class OutInsecureStream;
	template<typename _ChT>class Logger;
	template<typename _ChT>class LoggerObject;
}

#define __UL_AN_PREFIX "[UL DEV] "
#define _UL_AN_ERR(...) _k_an_add_p(__UL_AN_PREFIX, _k_e_an, C0, __VA_ARGS__)
#define _UL_AN_WAR(...) _k_an_add_p(__UL_AN_PREFIX, _k_w_an, NA, __VA_ARGS__)
#define _UL_AN_INF(...) _k_an_add_p(__UL_AN_PREFIX, _k_i_an, NA, __VA_ARGS__)

#define UL_AN_TODO(...) _k_an_add_p(" TODO: ",_k_i_an,TODO,__VA_ARGS__)
#define UL_AN_MUST(...) _k_an_add_p("Must be done before release: ",_k_i_an,TODO,__VA_ARGS__)
#define UL_AN_NOTE(...) _k_an_add_p("note: ", _k_i_an,N,__VA_ARGS__)

#ifndef _UL_SILENT_DEPRECATED_OPLITERAL_ULS
#define _UL_DEPRECATED_OPLITERAL_ULS [[deprecated( \
		"This operator should not be used due to its performance lacks. This will be updated !"\
	)]]
#endif


namespace ul {
#ifdef UNICODE
#error "UNICODE is not yet supported ! Waiting for stdlib to make a clean and cross platform wild char convertion"
	using ulchar_t = wchar_t;
#define UL_S(x) L##x
#define ulcout std::wcout
#define ulcerr std::wcerr
	std::wstring operator""_uls(const char* ch);
#else
	using ulchar_t = char;
#define UL_S(x) x
#define ulcout std::cout
#define ulcerr std::cerr

	_UL_DEPRECATED_OPLITERAL_ULS std::string operator""_uls(const char* ch);
#endif // UNICODE
	//Used for file relative
	using ulfchar_t = ulchar_t;
#define UL_FS(x) x
}