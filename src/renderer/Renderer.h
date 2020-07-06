#pragma once
#include "Shader.h"
//#include "Image.h"
#include "Camera.h"
#include "Texture.h"
#include "StaticVertices.h"
#include "../utils/Logger.h"
//#include "../filesystem/AssetLocation.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//#include "../physic/PhysicManager.h"


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

		int render(AssetsManager& assetManager, size_t shaderLocationId/*, PhysicManager& physicManager*/);

		void loadBasicAssets();

		inline void freeGlfw();

		void processInput(GLFWwindow* window);

		void frameBufferSizeC(GLFWwindow* window, int width, int height);
		void mouseC(GLFWwindow* window, double xpos, double ypos);
		void scrollC(GLFWwindow* window, double xoffset, double yoffset);

		struct IndirectDrawCmd {
			GLuint vertexCount;
			GLuint instanceCount;
			GLuint firstIndex;
			GLuint baseVertex;
			GLuint baseInstance;
		};

		private:
			void loadModelMatrices();

			int initGlfw();

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