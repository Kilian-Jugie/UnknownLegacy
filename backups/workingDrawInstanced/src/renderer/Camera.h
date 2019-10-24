#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		// Constructor with vectors
		Camera(glm::vec3 position = glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3 up = glm::vec3{ 0.0f, 1.0f, 0.0f },
			float yaw = YAW, float pitch = PITCH) noexcept 
			: m_Front{ glm::vec3{0.0f, 0.0f, -1.0f} }, m_MovementSpeed{ SPEED }, m_MouseSensitivity{ SENSITIVITY }, m_Zoom{ ZOOM },
			m_Position{ position }, m_WorldUp{ up }, m_Yaw{ yaw }, m_Pitch{ pitch } {
			updateCameraVectors();
		}

		// Constructor with scalar values
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) noexcept :
			m_Front{ glm::vec3{0.0f, 0.0f, -1.0f} }, m_MovementSpeed{ SPEED }, m_MouseSensitivity{ SENSITIVITY }, m_Zoom{ ZOOM },
			m_Position{ glm::vec3{posX, posY, posZ } }, m_WorldUp{ glm::vec3(upX, upY, upZ) }, m_Yaw{ yaw }, m_Pitch{ pitch } {
			updateCameraVectors();
		}

		// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
		glm::mat4 getViewMatrix() noexcept {
			return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		}

		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void processKeyboard(CameraMovement direction, float deltaTime) noexcept {
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

		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) noexcept {
			xoffset *= m_MouseSensitivity;
			yoffset *= m_MouseSensitivity;

			m_Yaw += xoffset;
			m_Pitch += yoffset;

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch) {
				if (m_Pitch > 89.0f)
					m_Pitch = 89.0f;
				if (m_Pitch < -89.0f)
					m_Pitch = -89.0f;
			}

			// Update Front, Right and Up Vectors using the updated Euler angles
			updateCameraVectors();
		}

		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void processMouseScroll(float yoffset) noexcept {
			if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
				m_Zoom -= yoffset;
			if (m_Zoom <= 1.0f)
				m_Zoom = 1.0f;
			if (m_Zoom >= 45.0f)
				m_Zoom = 45.0f;
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
		// Calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors() noexcept;

		// Camera Attributes
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
		// Euler Angles
		float m_Yaw;
		float m_Pitch;
		// Camera options
		float m_MovementSpeed;
		float m_MouseSensitivity;
		float m_Zoom;
	};
}