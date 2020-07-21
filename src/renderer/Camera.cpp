#include "Camera.h"

namespace ul {
	void Camera::processKeyboard(CameraMovement direction, float deltaTime) noexcept {
		float velocity = m_MovementSpeed * deltaTime;
		if (direction == CameraMovement::FORWARD)
			m_Position += m_Front * velocity;
		if (direction == CameraMovement::BACKWARD)
			m_Position -= m_Front * velocity;
		if (direction == CameraMovement::LEFT)
			m_Position -= m_Right * velocity;
		if (direction == CameraMovement::RIGHT)
			m_Position += m_Right * velocity;
	}
	
	void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) noexcept {
		xoffset *= m_MouseSensitivity;
		yoffset *= m_MouseSensitivity;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		if (constrainPitch) {
			if (m_Pitch > 89.0f)
				m_Pitch = 89.0f;
			if (m_Pitch < -89.0f)
				m_Pitch = -89.0f;
		}

		updateCameraVectors();
	}
	
	void Camera::processMouseScroll(float yoffset) noexcept {
		if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
			m_Zoom -= yoffset;
		if (m_Zoom <= 1.0f)
			m_Zoom = 1.0f;
		if (m_Zoom >= 45.0f)
			m_Zoom = 45.0f;


	}
	void Camera::updateCameraVectors() noexcept {
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

	}
}