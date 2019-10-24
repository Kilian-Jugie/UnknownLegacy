#include "ErrorManager.h"
#include "Error.h"

namespace ul {
	ErrorManager::ErrorManager() {
		m_Callbacks.resize(static_cast<const size_t>(ErrorLevel::__LEVEL_COUNT));
	}

	void ErrorManager::addCallback(ErrorLevel level, callback_t callback) {
		const size_t iLevel{ static_cast<const size_t>(level) };
		m_Callbacks[iLevel].push_back(callback);
	}

	void ErrorManager::error(Error& err) {
		const size_t iLevel{ static_cast<const size_t>(err.getLevel()) };
		for (auto c : m_Callbacks[iLevel])
			if (!c(err)) return;
	}
	void ErrorManager::copyError(Error err) {
		this->error(err);
	}
}