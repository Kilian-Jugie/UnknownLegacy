#pragma once
#include <string>
#include <codecvt>

//#define UL_USE_WCHAR

namespace ul {

#ifdef UL_USE_WCHAR
	using char_t = wchar_t;
#define UL_STR(x) L##x
#else
	using char_t = char;
#define UL_STR(x) x
#endif // UL_USE_WCHAR
	using string_t = std::basic_string<char_t>;

	class ulString : public string_t {
	public:
		template<typename _StrTy, typename = std::enable_if_t<std::is_constructible_v<string_t>>>
		ulString(_StrTy&& str) : string_t(std::forward<_StrTy>(str)) {}
		
#ifdef UL_USE_WCHAR
		

		operator std::string() {
			static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			return converter.to_bytes(this->data());
		}
#else
		operator std::wstring() {
			static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			return converter.from_bytes(this->data());
		}
#endif // UL_USE_WCHAR

	};

	static std::string wtos(const std::wstring& ws) {
		static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(ws);
	}

	static std::wstring stow(const std::string& s) {
		static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.from_bytes(s);
	}
}

