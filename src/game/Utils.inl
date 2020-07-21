#pragma once
#include "Utils.h"
#include "Position.h"
#include "world/InstancedObject.h"
#include <glm/matrix.hpp>

namespace ul {
    template<typename _P, typename>
    inline InstancedObject& ul::Utils::translate(InstancedObject& object, _P&& pos) {
        object.m_Model = glm::translate(object.m_Model, std::forward<Position>(translation));
        return object;
    }

    template<typename _P, typename>
    inline InstancedObject& ul::Utils::setPosition(InstancedObject& object, _P&& pos) {
        object.getModel()[3] = std::forward<Position>(pos);
        return object;
    }

    const Position& Utils::position(InstancedObject& object) {
        return Position(object.getModel()[3]);
    }

    inline bool Utils::isNameResolved(std::string name) {
        return name.find_first_of(':') != std::string::npos;
    }

    inline std::string Utils::getNameDomain(std::string name) {
        return name.substr(0, name.find_first_of(':'));
    }

    inline std::string Utils::getNameValue(std::string name) {
        return name.substr(name.find_first_of(':')+1);
    }

    inline std::pair<std::string, std::string> Utils::splitName(std::string name) {
        auto pos = name.find_first_of(':');
        return { name.substr(0, pos), name.substr(pos + 1) };
    }



}