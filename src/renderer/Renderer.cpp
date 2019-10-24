#include "Renderer.h"
#include "Mesh.h"
#include "RenderGroup.h"

namespace ul {
	void Renderer::loadModelMatrices() {
		for (size_t x(0u); x < DEBUG_RENDER_X; ++x) {
			for (size_t z(0u); z < DEBUG_RENDER_Y; ++z) {
				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
				model = glm::scale(model, glm::vec3(1.f));
				model = glm::rotate(model, 0.f, glm::vec3(1.f));
				m_ModelMatrices[x * DEBUG_RENDER_X + z] = model;
			}
		}
	}

	int Renderer::render(AssetsManager& assetManager, size_t shaderLocationId, PhysicManager& physicManager) {
		if (!m_isGlfwLoaded) {
			auto _inist = initGlfw();
			if (_inist != 0)
				return _inist;
		}

		m_Camera = new Camera{ physicManager.getCameraBody({0.f,0.f,0.f}) };

		linf << "Loading model matrices\n";
		loadModelMatrices();
		linf << "Done, compiling shaders\n";
		
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		//glFrontFace(GL_CW);

		linf << "Shaders location: " << assetManager.getLocation(shaderLocationId).getPath().string() << "\n";
		linf << "Vertex path: " << assetManager.getLocation(shaderLocationId).file("vertex.glsl") << "\n";
		linf << "Fragment path: " << assetManager.getLocation(shaderLocationId).file("fragment.glsl") << "\n";

		Shader shader(assetManager.getLocation(shaderLocationId).file("vertex.glsl").c_str(),
			assetManager.getLocation(shaderLocationId).file("fragment.glsl").c_str());

		linf << "Done, filling VAO, VBO, (EBO)\n";

		Mesh cube{ gCubeVertices, sizeof(gCubeVertices), gCubeIndices, sizeof(gCubeIndices)};

		RenderGroup cubeGroup{ static_cast<const Mesh&>(cube), DEBUG_RENDER_AMOUNT, m_ModelMatrices };

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		linf << "Done, loading texture\n";

		Texture tex{ "cobblestone.png" };

		shader.use();
		shader.setInt("texture1", 0);

		linf << "Done, Generating GPUs draw commands\n";

		IndirectDrawCmd testDraw;
		testDraw.vertexCount = 36;
		testDraw.instanceCount = DEBUG_RENDER_AMOUNT;
		testDraw.firstIndex = 0;
		testDraw.baseInstance = 0;
		testDraw.baseVertex = 0;


		unsigned indirectBuffer;

		glGenBuffers(1, &indirectBuffer);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(testDraw), &testDraw, GL_STATIC_DRAW);
		
		
		linf << "Done, entering main render loop\n";

		int currentS = 0;
		unsigned frameCount = 0;

		while (!glfwWindowShouldClose(m_Window)) {
			float currentFrame = glfwGetTime();
			m_DeltaTime = currentFrame - m_LastFrame;
			m_LastFrame = currentFrame;
			++frameCount;

			physicManager.simulate(m_DeltaTime);

			processInput(m_Window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex.getId());

			shader.use();

			glm::mat4 projection = glm::perspective(glm::radians(m_Camera->getZoom()), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 100.0f);
			shader.setMat4("projection", projection);

			glm::mat4 view = m_Camera->getViewMatrix();
			shader.setMat4("view", view);

			if ((int)(currentFrame + 1.f) > currentS) {
				currentS = currentFrame + 1.f;
				linf << frameCount << "FPS\n";
				frameCount = 0;
			}

			glBindVertexArray(cube.getVAO());

			glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, 0);

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