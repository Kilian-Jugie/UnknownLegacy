#pragma once
#include <string>
#include "../global.h"

namespace ul {
	enum class ErrorLevel {
		FATAL,
		ERROR,
		WARNING,
		INFO,
		DEBUG,

		__LEVEL_COUNT
	};

	class Error {
	public:
		using string_t = std::basic_string<ulchar_t>;

		Error(ErrorLevel level, string_t description) noexcept
			: m_Level{ level }, m_Description{ std::move(description) } {}

		Error(const Error&) = default;
		Error(Error&&) = default;
		~Error() = default;

		inline ErrorLevel getLevel() const noexcept {
			return m_Level;
		}

		inline const std::string& getDescription() const noexcept {
			return m_Description;
		}

	private:
		ErrorLevel m_Level;
		string_t m_Description;
	};
}