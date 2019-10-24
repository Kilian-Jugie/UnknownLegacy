#pragma once
#include "Shader.h"
//#include "Image.h"
#include "Camera.h"
#include "Texture.h"
#include "StaticVertices.h"
#include "../utils/Logger.h"
#include "../filesystem/AssetLocation.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "../physic/PhysicManager.h"


#define UL_ENABLE_ANTI_ALIASING
#define UL_ANTI_ALIASING MSAA
#define UL_ANTI_ALIASING_POWER 4

#if defined(UL_ENABLE_ANTI_ALIASING) && (!defined(UL_ANTI_ALIASING) || !defined(UL_ANTI_ALIASING_POWER))
#error When UL_ENABLE_ANTI_ALIASING is defined, UL_ANTI_ALIASING and UL_ANTI_ALIASING_POWER must be defined
#endif

namespace ul {

	void logMat4(glm::mat4 model);

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	class Renderer {
		

		Renderer() : m_Window{ nullptr } {
		}

		struct IndirectDrawCmd {
			GLuint vertexCount;
			GLuint instanceCount;
			GLuint firstIndex;
			GLuint baseVertex;
			GLuint baseInstance;
		};

		using physcallback_t = void(*)(float,void*);

	public:
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		static Renderer& getRenderer() {
			static Renderer r;
			return r;
		}

		~Renderer() {
			if (m_isGlfwLoaded) freeGlfw();
		}

		int render(AssetsManager& assetManager, size_t shaderLocationId, PhysicManager& physicManager);

		inline void freeGlfw() {
			glfwTerminate();
			m_isGlfwLoaded = false;
		}

		void processInput(GLFWwindow* window) {
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
			float cameraSpeed = 2.5f * m_DeltaTime;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				m_Camera->processKeyboard(CameraMovement::FORWARD, m_DeltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				m_Camera->processKeyboard(CameraMovement::BACKWARD, m_DeltaTime);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				m_Camera->processKeyboard(CameraMovement::LEFT, m_DeltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				m_Camera->processKeyboard(CameraMovement::RIGHT, m_DeltaTime);
		}

		void frameBufferSizeC(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		void mouseC(GLFWwindow* window, double xpos, double ypos) {
			if (firstMouse) {
				m_MouseLastX = static_cast<float>(xpos);
				m_MouseLastY = static_cast<float>(ypos);
				firstMouse = false;
			}

			float xoffset = xpos - m_MouseLastX;
			float yoffset = m_MouseLastY - ypos; // reversed since y-coordinates go from bottom to top
			m_MouseLastX = xpos;
			m_MouseLastY = ypos;

			m_Camera->processMouseMovement(xoffset, yoffset);
		}


		void scrollC(GLFWwindow* window, double xoffset, double yoffset) {
			m_Camera->processMouseScroll(yoffset);
		}

		private:
			void loadModelMatrices();

			int initGlfw() {
				linf << "Initializing GLFW...\n";
				glfwInit();

				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef UL_ENABLE_ANTI_ALIASING
				glfwWindowHint(GLFW_SAMPLES, UL_ANTI_ALIASING_POWER);                                      
#endif // UL_ENABLE_ANTIALISATING

				m_Window = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, "UnknownLegacy", nullptr, nullptr);
				if (m_Window == nullptr) {
					lfat << "Failed to create GLFW window\n";
					glfwTerminate();
					return -1;
				}
				glfwMakeContextCurrent(m_Window);
				glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
				glfwSetCursorPosCallback(m_Window, mouse_callback);
				glfwSetScrollCallback(m_Window, scroll_callback);
				glfwSwapInterval(0);

				glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

				m_isGlfwLoaded = true;

				if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
					lfat << "Failed to initialize GLAD\n";
					return -1;
				}

				glEnable(GL_DEPTH_TEST);

#ifdef UL_ENABLE_ANTI_ALIASING
#if UL_ANTI_ALIASING == MSAA
				glEnable(GL_MULTISAMPLE);
#else
#error Incorrect Anti-Aliasing technology
#endif // 

#endif // UL_ENABLE_ANTI_ALIASING




				linf << "Initialization done !\n";
				return 0;
			}

			private:
			const unsigned int m_ScreenWidth = 800;
			const unsigned int m_ScreenHeight = 600;

			float m_DeltaTime = 0.0f; // Time between current frame and last frame
			float m_LastFrame = 0.0f; // Time of last frame

			glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

			Camera* m_Camera;
			bool firstMouse = true;
			float m_MouseLastX = m_ScreenWidth / 2.f;
			float m_MouseLastY = m_ScreenHeight / 2.f;

			bool m_isGlfwLoaded = false;

			glm::mat4 m_ModelMatrices[DEBUG_RENDER_AMOUNT];

			GLFWwindow* m_Window;
	};

	
}