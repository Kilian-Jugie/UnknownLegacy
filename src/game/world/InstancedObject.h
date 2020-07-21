#pragma once
#include <Types.h>
#include "../Position.h"
#include "InstancedObjectType.h"
#include "../GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ul {
	/*
		TODO :
		Use of 3*vec3 instead of mat4 with size & rotation
		as references to instanciate only 1 size & rotation with
		default values 1,1,1 & 0,0,0
		Ops may be done to use this as mat4

	*/
	struct InstancedObject {
		InstancedObject(glm::mat4 mod, const WorldObject& obj): m_Model{std::move(mod)}, m_Obj{obj} {}

		inline const glm::mat4& getModel() const noexcept {
			return m_Model;
		}

		inline const WorldObject& getWorldObject() const noexcept {
			return m_Obj;
		}

	private:
		glm::mat4 m_Model;
		const WorldObject& m_Obj;
	};
}