#include "Error.h"

namespace ul {
	ErrorLevel Error::getLevel() const noexcept {
		return m_Level;
	}

	const std::string& Error::getDescription() const noexcept {
		return m_Description;
	}


}
