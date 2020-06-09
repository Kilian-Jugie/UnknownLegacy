#pragma once
#include <Types.h>

namespace ul {
	class GameObject {
	public:

		GameObject(ulString name) noexcept : m_Uoid{ ++s_CurrentUoid }, m_Name{ std::move(name) } {}

		inline id_t getUniqueObjectId() const noexcept {
			return m_Uoid;
		}

	private:
		inline static id_t s_CurrentUoid = 0;
		const id_t m_Uoid;
		const ulString m_Name;
	};
}