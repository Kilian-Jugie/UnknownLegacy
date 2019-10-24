#include "global.h"
#include <codecvt>

#ifdef UNICODE
std::wstring ul::operator""_uls(const char* ch) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> _converter;
	return _converter.from_bytes(std::string(ch));
}
#else
std::string ul::operator""_uls(const char* ch) {
	return std::string(ch);
}

#endif // UNICODE


