#pragma once
#include "utils/Types.h"
#include "utils/StringUtils.h"

namespace ul {
	class GameObject {
	public:
		GameObject(ulString domain, ulString name) noexcept : m_Uoid{ ++s_CurrentUoid }, m_Name{ std::move(name) }, m_Domain{ std::move(domain) } {}

		virtual void initialize() noexcept = 0;

		inline id_t getUniqueObjectId() const noexcept {
			return m_Uoid;
		}

		inline const ulString& getName() noexcept {
			return m_Name;
		}

		inline const ulString& getDomain() noexcept {
			return m_Domain;
		}

	private:
		inline static id_t s_CurrentUoid = 0;
		const id_t m_Uoid;
		const ulString m_Name;
		const ulString m_Domain;
	};
}