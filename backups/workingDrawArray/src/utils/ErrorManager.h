#pragma once
#include <vector>
#include "Error.h"
#include "Logger.h"


namespace ul {
	class ErrorManager {
	public:
		using callback_t = bool(*)(Error&);

		ErrorManager();
		ErrorManager(const ErrorManager&) = delete;
		ErrorManager(ErrorManager&&) = default;

		ErrorManager& operator=(const ErrorManager&) = delete;
		ErrorManager& operator=(ErrorManager&&) = default;
		
		void addCallback(ErrorLevel level, callback_t callback);

		void error(Error& err);
		void copyError(Error err);
		

	private:
		std::vector<std::vector<callback_t>> m_Callbacks;
	};
}