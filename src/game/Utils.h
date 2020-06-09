#pragma once
#include "fwd.h"
#include <type_traits>

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
	};
}

#include "Utils.inl"