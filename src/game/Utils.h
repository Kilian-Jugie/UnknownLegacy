#pragma once
#include "fwd.h"
#include <type_traits>
#include <codecvt>

namespace ul {
	class Utils {
	public:
		template<typename _P,
			typename = std::enable_if_t<std::is_constructible_v<Position, std::decay_t<_P>>>>
		static InstancedObject& translate(InstancedObject& object, _P&& translation);

		template<typename _P,
			typename = std::enable_if_t<std::is_constructible_v<Position, std::decay_t<_P>>>>
			static InstancedObject& setPosition(InstancedObject& object, _P&& pos);

		static const Position& position(InstancedObject& object);

		static bool isNameResolved(std::string name);
		static std::string getNameDomain (std::string name);
		static std::string getNameValue(std::string name);
		static std::pair<std::string, std::string> splitName(std::string name);
	};
}

#include "Utils.inl"