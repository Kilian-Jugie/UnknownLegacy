#pragma once
#include <Types.h>
#include "../Position.h"
#include "InstancedObjectType.h"
#include "../GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ul {
	/*class InstancedObject {
	public:
		static InstancedObject Null;

		InstancedObject(Position position) noexcept : m_Model{ 1.f } {
			m_Model = glm::translate(m_Model, (glm::vec3)position);
		}

		InstancedObject(const glm::mat4& model) noexcept : m_Model(model) {}


		Position getPosition() const noexcept {
			return m_Model[3];
		}

		const glm::mat4& getModel() const noexcept {
			return m_Model;
		}

		void translate(const Position& translation) noexcept {
			m_Model = glm::translate(m_Model, (glm::vec3)translation);
			onTranslated(translation);
		}

		void setPosition(const Position& position) noexcept {
			m_Model[3] = (glm::vec4)position;
		}

		virtual void onTranslated(const Position& translation) {}

	private:

		glm::mat4 m_Model;
	};

	InstancedObject InstancedObject::Null{ glm::mat4{1.f} };*/

	/*
		TODO :
		Use of 3*vec3 instead of mat4 with size & rotation
		as references to instanciate only 1 size & rotation with
		default values 1,1,1 & 0,0,0
		Ops may be done to use this as mat4

	*/
	struct InstancedObject {
		glm::mat4 model;
	};
}