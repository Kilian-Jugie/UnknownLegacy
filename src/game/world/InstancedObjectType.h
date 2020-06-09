#pragma once
#include <Types.h>
#include <type_traits>

namespace ul {
	class InstancedObjectType {
	public:
		static InstancedObjectType Null;

		template<typename _StrT, typename = std::enable_if_t<std::is_constructible_v<string_t, _StrT>>>
		InstancedObjectType(_StrT&& name) : m_Id(++s_CurrentId), m_Name(std::forward<string_t>(name)) {}

		id_t getId() const noexcept {
			return m_Id;
		}

		const string_t& getName() const noexcept {
			return m_Name;
		}

	private:
		template<typename _StrT, typename = std::enable_if_t<std::is_constructible_v<string_t, _StrT>>>
		InstancedObjectType(_StrT&& name, id_t id) : m_Id(id), m_Name(std::forward<string_t>(name)) {}

	protected:
		const id_t m_Id;
		const string_t m_Name;

	private:
		static id_t s_CurrentId;
	};

	InstancedObjectType InstancedObjectType::Null = { UL_STR("NULL"), 0 };
	id_t InstancedObjectType::s_CurrentId = 0;

	namespace WorldObjectTypes {
		InstancedObjectType BLOCK{ UL_STR("BLOCK") };
	}
}