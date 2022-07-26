#pragma once
#include "global.h"
#include "Stream.h"
#include <string>
#include <iostream>


/*
	TODO: uniformize using of char_t inside classes
	TO DEBATE: 
		- Use of time with std::wcsftime because it will
	make all of this VERY VERY slow due to bery bad std api
	for this function. Or another solution should be used ?
		- Is this lib part really performance critical ? Will
	it be keeped after game dev ? Will it only leave for
	debugging ?

*/

namespace ul {

	/*enum class LogCategory {
		ERROR,
		OTHER
	};*/
	/*

	constexpr size_t _gDateLength = 19;

	class LogCategory {
	public:
		using char_t = wchar_t;
		using ostream_t = std::basic_ostream<char_t>;

		LogCategory(ostream_t& out) noexcept : m_Out{ out } {}
		LogCategory() = delete;
		LogCategory(const LogCategory&) noexcept = default;
		LogCategory(LogCategory&&) noexcept = default;

		constexpr ostream_t& getOut() const noexcept { return m_Out; }
	private:
		ostream_t& m_Out;
	};
	*/
	/*
		_lcError should use std::wcerr but to do that we must
		flush the std::wcout buffer before using std::wcerr instead
		some messages from std::wcout "displayed before" will be
		shown after... We could adapt code modularity to allow that
	*/
	/*
	static const LogCategory _lcError{ std::wcout };
	static const LogCategory _lcOther{ std::wcout };

	class LogLevel {
	public:
		using char_t = wchar_t;
		using string_t = std::basic_string<char_t>;

		LogLevel(const LogCategory& category, string_t label);
		LogLevel(const LogLevel&) = default;
		LogLevel(LogLevel&&) noexcept = default;
		~LogLevel() = default;

		auto getLabel() const noexcept {
			return m_Label;
		}

		auto getCategory() const noexcept {
			return m_Category;
		}

	private:
		const string_t m_Label;
		const LogCategory& m_Category;
	};
	
	static LogLevel _lErrorLog{ _lcError, L"ERROR" };
	static LogLevel _lWarningLog{ _lcOther, L"WARNING" };
	static LogLevel _lInfoLog{ _lcOther, L"INFO" };
	static LogLevel _lDebugLog{ _lcOther, L"DEBUG" };

	class Logger {
	public:
		using char_t = wchar_t;
		using string_t = std::basic_string<wchar_t>;
		using stream_t = OutInsecureStream<char_t>;

		Logger(LogLevel level, string_t filename) : m_Level{ std::move(level) }, m_Stream{ std::move(filename) } {}
		*/
	/*
		Logger& operator<<(string_t s) {
			addDateToString(s);
			m_Stream << s;
			m_Level.getCategory().getOut() << s;
			return *this;
		}
		
		Logger& emplace(string_t& s) {
			addDateToString(s);
			m_Stream << s;
			m_Level.getCategory().getOut() << s;
			return *this;
		}
		Logger& push(string_t&& s) {
			m_Level.getCategory().getOut() << s;
			m_Stream << std::move(s);
			return *this;
		}

	private:
		
		void addDateToString(string_t& s) {
			for (string_t::iterator it(s.begin()); it!=s.end(); ++it) {
				if ((*it) == '\n') {
					++it;
					static auto prefix{ L"["+m_Level.getLabel()+L"] " };
					it = s.insert(it, prefix.begin(), prefix.end());
					
					//auto nfTime = std::put_time<char_t>(std::localtime(nullptr), L"%m/%d/%Y %H:%M:%S");

				}
			}
		}

		const LogLevel m_Level;
		stream_t m_Stream;
	};

	static Logger linf{ _lInfoLog, L"global.log" };
	static Logger lwar{ _lErrorLog, L"global.log" };
	static Logger lerr{ _lWarningLog, L"global.log" };

	static void clearLogs() {
		remove("global.log");
	}

	static void initLogs() {
		linf << L"";
		lerr << L"";
		lwar << L"";
	}
	*/

	template<typename _ChT>class LoggerObject;

	template<typename _ChT>class Logger {
	public:
		//FileStream_t
		using FStream_t = OutInsecureStream<_ChT>;
		//ScreenStream_t
		using SStream_t = std::basic_ostream<_ChT>;
		using string_t = std::basic_string<_ChT>;


		Logger(string_t filename, SStream_t& screen) : m_FStream{ std::move(filename) }, m_SStream{ screen }, m_LastLogObj{ nullptr } {}
		Logger(const Logger&) noexcept = default;
		Logger(Logger&&) noexcept  = default;


		inline void log(string_t s, const string_t& prefix, LoggerObject<_ChT>& lo) {
			put(s,prefix, lo);
		}

		void put(string_t& s, const string_t& prefix, LoggerObject<_ChT>& lo);

	private:
		static bool m_WasLastEndl;

		void format(string_t& s, const string_t& prefix) {
			if (m_WasLastEndl) {
				s.insert(s.begin(), prefix.begin(), prefix.end());
				m_WasLastEndl = false;
			}

			//s.end()-1 must be recalculated each iteration because s is modified
			for (typename string_t::iterator it(s.begin()); it != (s.end() - 1); ++it) {
				if ((*it) == UL_S('\n')) {
					++it;
					it = s.insert(it, prefix.begin(), prefix.end());
					it += prefix.size() - 1;
				}
			}
			if (s[s.size() - 1] == UL_S('\n')) m_WasLastEndl = true;
		}

		FStream_t m_FStream;
		SStream_t& m_SStream;
		LoggerObject<_ChT>* m_LastLogObj;
	};

	template<typename _ChT>
	bool Logger<_ChT>::m_WasLastEndl = true;

	template<typename _ChT>class LoggerObject {
	public:
		using string_t = std::basic_string<_ChT>;
		
		LoggerObject(string_t prefix, Logger<_ChT>& l) : m_Prefix{std::move(prefix)}, m_Logger{l} {}
		LoggerObject(const LoggerObject&) noexcept = default;
		LoggerObject(LoggerObject&&) noexcept = default;

		LoggerObject& operator<<(const string_t& s) {
			m_Logger.log(s, m_Prefix, *this);
			return *this;
		}

		template<typename _Ty,
			typename = std::enable_if_t<std::is_arithmetic_v<_Ty>>>
		LoggerObject & operator<<(_Ty i) {
			m_Logger.log(std::to_string(i), m_Prefix, *this);
			return *this;
		}

	private:
		const string_t m_Prefix;
		Logger<_ChT>& m_Logger;
	};

	static Logger<ulchar_t> g_GlobalLogger{ UL_S("global.log"), ulcout };
	static LoggerObject<ulchar_t> linf{ UL_S("[INFO] "), g_GlobalLogger },
		lwar{ UL_S("[WARNING] "), g_GlobalLogger },
		lerr{ UL_S("[ERROR] "), g_GlobalLogger },
		lfat{ UL_S("[FATAL] "), g_GlobalLogger },
		ldeb{ UL_S("[DEBUG] "), g_GlobalLogger };

	template<typename _ChT>
	inline void Logger<_ChT>::put(string_t& s, const string_t& prefix, LoggerObject<_ChT>& lo) {
			if (&lo != m_LastLogObj) {
				m_LastLogObj = &lo;
				s.insert(s.begin(), UL_S('\n'));
			}
			format(s, prefix);
			m_FStream << s;
			m_SStream << s;
	}

}