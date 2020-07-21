#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../global.h"

#include <vector>

namespace ul {

	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum class CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// Default camera values
	constexpr float YAW = -90.f;
	constexpr float PITCH = 0.f;
	constexpr float SPEED = 2.5f;
	constexpr float SENSITIVITY = 0.1f;
	constexpr float ZOOM = 45.f;

	class Camera {
	public:

		Camera(glm::vec3 position = glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3 up = glm::vec3{ 0.0f, 1.0f, 0.0f },
			float yaw = YAW, float pitch = PITCH) noexcept 
			: m_Front{ glm::vec3{0.0f, 0.0f, -1.0f} }, m_MovementSpeed{ SPEED }, m_MouseSensitivity{ SENSITIVITY }, m_Zoom{ ZOOM },
			/*m_Position{ position },*/ m_WorldUp{ up }, m_Yaw{ yaw }, m_Pitch{ pitch } {
			updateCameraVectors();
		}

		~Camera() {
		}

		void processKeyboard(CameraMovement direction, float deltaTime) noexcept;
		void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) noexcept;
		void processMouseScroll(float yoffset) noexcept;

		inline glm::mat4 getViewMatrix() noexcept {
			return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		}

		inline glm::vec3 getPosition() {
			return m_Position;
		}

		inline void setPosition(glm::vec3 v) {
			m_Position = v;
		}

		inline void setSpeed(float newSpeed) noexcept {
			m_MovementSpeed = newSpeed;
		}

		inline void setSensivity(float newSensi) noexcept {
			m_MouseSensitivity = newSensi;
		}

		inline void setZoom(float newZoom) noexcept {
			m_Zoom = newZoom;
		}

		inline float getSpeed() const noexcept {
			return m_MovementSpeed;
		}

		inline float getSensivity() const noexcept {
			return m_MouseSensitivity;
		}

		inline float getZoom() const noexcept {
			return m_Zoom;
		}

	private:
		void updateCameraVectors() noexcept;

		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;

		float m_Yaw;
		float m_Pitch;

		float m_MovementSpeed;
		float m_MouseSensitivity;
		float m_Zoom;
	};
}