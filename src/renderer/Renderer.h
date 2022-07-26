#pragma once
#include "Shader.h"
#include "Camera.h"
#include "StaticVertices.h"
#include "utils/Logger.h"
#include "TextureManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define UL_ENABLE_ANTI_ALIASING
#define UL_ANTI_ALIASING MSAA
#define UL_ANTI_ALIASING_POWER 4

#if defined(UL_ENABLE_ANTI_ALIASING) && (!defined(UL_ANTI_ALIASING) || !defined(UL_ANTI_ALIASING_POWER))
#error When UL_ENABLE_ANTI_ALIASING is defined, UL_ANTI_ALIASING and UL_ANTI_ALIASING_POWER must be defined
#endif

namespace ul {
	class Mesh;
	class Shader;
	class TextRenderer;

	void logMat4(glm::mat4 model);

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	class Renderer {
		Renderer();

		using physcallback_t = void(*)(float,void*);
	public:
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		static Renderer& getInstance() {
			static Renderer r;
			return r;
		}

		~Renderer();

		int render(AssetsManager& assetManager, size_t shaderLocationId/*, PhysicManager& physicManager*/);
		void loop();

		void loadBasicAssets();

		inline void freeGlfw();

		void processInput(GLFWwindow* window);

		void frameBufferSizeC(int width, int height);
		void mouseC(double xpos, double ypos);
		void scrollC(double yoffset);

		inline TextureManager& getTextureManager() noexcept {
			return m_TextureManager;
		}

		struct IndirectDrawCmd {
			GLuint vertexCount;
			GLuint instanceCount;
			GLuint firstIndex;
			GLuint baseVertex;
			GLuint baseInstance;
		};

		private:
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
			TextureManager m_TextureManager;
			Mesh* m_ChunksMesh;
			Shader* m_Shaders;
			TextRenderer* m_TextRenderer;
	};

	
}