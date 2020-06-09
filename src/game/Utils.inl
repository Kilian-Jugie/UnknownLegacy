#pragma once
#include "Utils.h"
#include "Position.h"
#include "world/InstancedObject.h"
#include <glm/matrix.hpp>

namespace ul {
    template<typename _P, typename>
    inline InstancedObject& ul::Utils::translate(InstancedObject& object, _P&& pos) {
        object.model = glm::translate(object.model, std::forward<Position>(translation));
        return object;
    }

    template<typename _P, typename>
    inline InstancedObject& ul::Utils::setPosition(InstancedObject& object, _P&& pos) {
        object.model[3] = std::forward<Position>(pos);
        return object;
    }

    const Position& Utils::position(InstancedObject& object) {
        return Position(object.model[3]);
    }

}