#include "Renderer.h"

namespace ul {
	int Renderer::render(AssetsManager assetManager, size_t shaderLocationId) {
		if (!m_isGlfwLoaded) {
			auto _inist = initGlfw();
			if (_inist != 0)
				return _inist;
		}
		linf << "Loading model matrices\n";
		loadModelMatrices();
		linf << "Done, compiling shaders\n";
		//glm::vec3 cubePositions[16 * 16];
		
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		//glFrontFace(GL_CW);

		Shader shader("vertex.glsl", "fragment.glsl");

		linf << "Done, filling VAO\n";

		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		unsigned int matricesBuffer;
		glGenBuffers(1, &matricesBuffer);

		unsigned int VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(gCubeVertices), gCubeVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, matricesBuffer);
		glBufferData(GL_ARRAY_BUFFER, DEBUG_RENDER_AMOUNT * sizeof(glm::mat4), &m_ModelMatrices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		linf << "Done, loading texture\n";

		Texture tex{ "cobblestone.png" };

		shader.use();
		shader.setInt("texture1", 0);

		linf << "Done, entering main render loop\n";

		int currentS = 0;
		unsigned frameCount = 0;

		while (!glfwWindowShouldClose(m_Window)) {
			//for(int i(0); i<3; ++i) {
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			++frameCount;

			processInput(m_Window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex.getId());

			shader.use();

			glm::mat4 projection = glm::perspective(glm::radians(m_Camera.getZoom()), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 100.0f);
			shader.setMat4("projection", projection);

			glm::mat4 view = m_Camera.getViewMatrix();
			shader.setMat4("view", view);

			if ((int)(currentFrame + 1.f) > currentS) {
				currentS = currentFrame + 1.f;
				linf << frameCount << "FPS\n";
				frameCount = 0;
			}

			glBindVertexArray(VAO);
			//for (unsigned int i = 0; i < DEBUG_RENDER_AMOUNT; i++) {
				//glm::mat4 model = glm::mat4(1.0f);
				//model = glm::translate(model, cubePositions[i]);
				//float angle = 20.0f * i;
				//shader.setMat4("model", model);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 36, DEBUG_RENDER_AMOUNT);



			///glDrawArrays(GL_TRIANGLES, 0, 36);
		//}


			glfwSwapBuffers(m_Window);
			glfwPollEvents();
		}

		freeGlfw();
		return 0;
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		Renderer::getRenderer().frameBufferSizeC(window, width, height);
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		Renderer::getRenderer().mouseC(window, xpos, ypos);
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		Renderer::getRenderer().scrollC(window, xoffset, yoffset);
	}

	void logMat4(glm::mat4 model) {
		linf << "[" << model[0][0] << ", " << model[0][1] << ", " << model[0][2] << ", " << model[0][3] << "\n"
			<< model[1][0] << ", " << model[1][1] << ", " << model[1][2] << ", " << model[1][3] << "\n"
			<< model[2][0] << ", " << model[2][1] << ", " << model[2][2] << ", " << model[2][3] << "\n"
			<< model[3][0] << ", " << model[3][1] << ", " << model[3][2] << ", " << model[3][3] << "]\n";
	}
}