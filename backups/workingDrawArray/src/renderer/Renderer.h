#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Image.h"
#include "Camera.h"
#include "StaticVertices.h"
#include "../utils/Logger.h"

namespace ul {

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	/*glm::vec3 cubePositions[] = {
		  glm::vec3(0.0f,  0.0f,  0.0f),
		  glm::vec3(1.0f,  0.0f, 0.0f),
		  glm::vec3(-1.5f, -2.2f, -2.5f),
		  glm::vec3(-3.8f, -2.0f, -12.3f),
		  glm::vec3(2.4f, -0.4f, -3.5f),
		  glm::vec3(-1.7f,  3.0f, -7.5f),
		  glm::vec3(1.3f, -2.0f, -2.5f),
		  glm::vec3(1.5f,  2.0f, -2.5f),
		  glm::vec3(1.5f,  0.2f, -1.5f),
		  glm::vec3(-1.3f,  1.0f, -1.5f)
	};*/

	class Renderer {
		const unsigned int SCR_WIDTH = 800;
		const unsigned int SCR_HEIGHT = 600;

		float deltaTime = 0.0f; // Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		Camera m_Camera{ glm::vec3(0.f, 0.f, 3.f) };
		bool firstMouse = true;
		float lastX = SCR_WIDTH / 2.0;
		float lastY = SCR_HEIGHT / 2.0;

		Renderer() = default;


	public:
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		static Renderer& getRenderer() {
			static Renderer r;
			return r;
		}


		int render() {
			glm::vec3 cubePositions[16 * 16];

			for (size_t x(0u); x < 16; ++x) {
				for (size_t z(0u); z < 16; ++z) {
					cubePositions[x*16+z] = glm::vec3((float)x, 0.0f, z);
				}
			}

			glfwInit();

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "UnknownLegacy", NULL, NULL);
			if (window == NULL) {
				lfat << "Failed to create GLFW window\n";
				glfwTerminate();
				return -1;
			}
			glfwMakeContextCurrent(window);
			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
			glfwSetCursorPosCallback(window, mouse_callback);
			glfwSetScrollCallback(window, scroll_callback);
			glfwSwapInterval(0);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				lfat << "Failed to initialize GLAD\n";
				return -1;
			}

			glEnable(GL_DEPTH_TEST);
			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_FRONT);
			//glFrontFace(GL_CW);

			Shader shader("vertex.glsl", "fragment.glsl");

			unsigned int VAO;
			glGenVertexArrays(1, &VAO);

			unsigned int VBO;
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(gCubeVertices), gCubeVertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

#ifdef WIREFRAME
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif // WIREFRAME

			unsigned int texture/*, texture2*/;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			Image imgContainer("cobblestone.png");

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgContainer.getWidth(), imgContainer.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgContainer.getData());
			glGenerateMipmap(GL_TEXTURE_2D);

			imgContainer.free();

			shader.use();
			shader.setInt("texture1", 0);
			
			int currentS = 0;
			unsigned frameCount = 0;

			while (!glfwWindowShouldClose(window)) {
				float currentFrame = glfwGetTime();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;
				++frameCount;

				if ((int)(currentFrame + 1.f) > currentS) {
					currentS = currentFrame + 1.f;
					linf << frameCount << "FPS\n";
					frameCount = 0;
				}

				processInput(window);

				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture);

				

				shader.use();

				glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
				shader.setMat4("projection", projection);

				

				glm::mat4 view = m_Camera.GetViewMatrix();
				shader.setMat4("view", view);

				glBindVertexArray(VAO);
				for (unsigned int i = 0; i < 16*16; i++)
				{
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, cubePositions[i]);
					float angle = 20.0f * i;
					shader.setMat4("model", model);

					glDrawArrays(GL_TRIANGLES, 0, 36);
				}

				
				glfwSwapBuffers(window);
				glfwPollEvents();
			}

			glfwTerminate();
			return 0;
		}

		void processInput(GLFWwindow* window) {
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
			float cameraSpeed = 2.5f * deltaTime;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				m_Camera.ProcessKeyboard(FORWARD, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				m_Camera.ProcessKeyboard(BACKWARD, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				m_Camera.ProcessKeyboard(LEFT, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				m_Camera.ProcessKeyboard(RIGHT, deltaTime);
		}

		void frameBufferSizeC(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		void mouseC(GLFWwindow* window, double xpos, double ypos)
		{
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
			lastX = xpos;
			lastY = ypos;

			m_Camera.ProcessMouseMovement(xoffset, yoffset);
		}


		void scrollC(GLFWwindow* window, double xoffset, double yoffset)
		{
			m_Camera.ProcessMouseScroll(yoffset);
		}
	};

	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		Renderer::getRenderer().frameBufferSizeC(window, width, height);
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		Renderer::getRenderer().mouseC(window, xpos, ypos);
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		Renderer::getRenderer().scrollC(window, xoffset, yoffset);
	}
}