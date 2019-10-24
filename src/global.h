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
#else
#define _UL_DEPRECATED_OPLITERAL_ULS
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

#if (defined(_DEBUG) || defined(DEBUG)) && !defined(UL_DEBUG)
#define UL_DEBUG
#endif // (_DEBUG) || defined(DEBUG)

#ifdef _MSC_VER
#define UL_VS _MSC_VER
#endif // _MSC_VER

#ifdef UL_VS
#define _UL_COMP_ASSUME(x) __assume(x)
#define _UL_COMP_UNREACH() _UL_COMP_ASSUME(0)
#define _UL_COMP_ASSERT_FUNC _wassert
#define _UL_COMP_FORCEINLINE __forceinline
#elif defined(UL_GCC)
#define _UL_COMP_FORCEINLINE __attribute__((always_inline))
#else
#define _UL_COMP_ASSUME(x) 
#define _UL_COMP_UNREACH()
#ifndef _UL_COMP_USER_NOASSERTFUNC_FALLBACK
#define _UL_COMP_NOASSERTFUNC_FALLBACK(msg,file,line) assert(0&&msg)
#endif //! _UL_COMP_USER_NOASSERTFUNC_FALLBACK
#define _UL_COMP_ASSERT_FUNC _UL_COMP_NOASSERTFUNC_FALLBACK
#define _UL_COMP_FORCEINLINE inline
#endif // UL_VS

#define _UL_STRINGIZE_(x) #x
#define _UL_STRINGIZE(x) _UL_STRINGIZE_(x)

#define _UL_WIDE_(x) L ## x
#define _UL_WIDE(x) _UL_WIDE_(x)

#define _UL_CAT_(a, b) a ## b
#define _UL_CAT(a, b) _UL_CAT_(a,b)


#ifdef UL_DEBUG
#include <assert.h>
#define UL_ASSERT_NW(expr, msg) (!!(expr))||(_UL_COMP_ASSERT_FUNC(msg, _UL_WIDE(__FILE__), static_cast<unsigned>(__LINE__)),0)
#define UL_ASSERT(expr, msg) UL_ASSERT_NW(expr, _UL_WIDE(msg))
#define UL_ASSUME(x) UL_ASSERT_NW(x, _UL_WIDE("Assuming ")_UL_WIDE(_UL_STRINGIZE(x))_UL_WIDE(" failed!"))
#define UL_UNREACHABLE() UL_ASSERT(0, "Unreachable code reached !!! THIS MUST BE CHANGED")
#define UL_FORCEINLINE
#define UL_INLINE
#define UL_NOEXCEPT noexcept
#else
#define UL_ASSERT_NW(expr, msg)
#define UL_ASSERT(expr, msg)
#define UL_ASSUME(x) _UL_COMP_ASSUME(x)
#define UL_UNREACHABLE() _UL_COMP_UNREACH()
#define UL_FORCEINLINE _UL_COMP_FORCEINLINE
#define UL_INLINE inline
#define UL_NOEXCEPT noexcept
#endif // UL_DEBUG

#define DEBUG_RENDER_X 128
#define DEBUG_RENDER_Y 128
#define DEBUG_RENDER_AMOUNT DEBUG_RENDER_X * DEBUG_RENDER_Y

#define UL_PHYSX_RELEASE(x) if(x) {x->release(); x = nullptr;}

#ifdef UL_DEBUG
#define UL_PHYSX_TRACK_ALLOCS
#endif // DEBUG

#define UL_PVD_HOST "127.0.0.1"

//TODO: Make this modular
#define UL_PHYSX_THREADS_NB 2
