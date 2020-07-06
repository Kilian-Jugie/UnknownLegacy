#include "Renderer.h"
#include "Mesh.h"
#include "RenderGroup.h"
#include "../game/world/Chunk.h"
#include "../ul.h"
#include "../utils/StringUtils.h"
#include "TextRenderer.h"
#include <fmt/format.h>

namespace ul {
	void Renderer::loadModelMatrices() {
		for (size_t x(0u); x < DEBUG_RENDER_X; ++x) {
			for (size_t z(0u); z < DEBUG_RENDER_Y; ++z) {
				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)z));
				model = glm::scale(model, glm::vec3(0.5f));
				//model = glm::rotate(model, 0.f, glm::vec3(1.f));
				m_ModelMatrices[x * DEBUG_RENDER_X + z] = model;
			}
		}
	}

	inline int Renderer::initGlfw() {
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

	int Renderer::render(AssetsManager& assetManager, size_t shaderLocationId/*, PhysicManager& physicManager*/) {
		if (!m_isGlfwLoaded) {
			auto _inist = initGlfw();
			if (_inist != 0)
				return _inist;
		}

		m_Camera = new Camera{ /*physicManager.getCameraBody({0.f,0.f,0.f})*/ };

		//linf << "Loading model matrices\n";
		loadModelMatrices();
		linf << "Done, compiling shaders\n";
		
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CCW);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		///linf << "Shaders location: " << assetManager.getLocation(shaderLocationId).getPath().string() << "\n";
		linf << "Vertex path: " << wtos(assetManager.getPath("vertex.glsl", shaderLocationId)) << "\n";
		linf << "Fragment path: " << wtos(assetManager.getPath("fragment.glsl", shaderLocationId)) << "\n";

		Shader shader(wtos(assetManager.getPath("vertex.glsl", shaderLocationId)).c_str(),
			wtos(assetManager.getPath("fragment.glsl", shaderLocationId)).c_str());

		linf << "Done, filling VAO, VBO, (EBO)\n";

		TextRenderer txt;
		txt.initialize(assetManager, shaderLocationId, { m_ScreenWidth, m_ScreenHeight });

		
		Mesh cube{ gCubeVertices, sizeof(gCubeVertices), gCubeTexCoords, sizeof(gCubeTexCoords), nullptr, 0/*sizeof(gCubeIndices)*/};

		Chunk ch{ {0,0,0} };

		Mesh tbm = /*gBlockHlMesh.toMesh();*/ ch.toMesh(nullptr); // gBlockHlMesh.getAsMesh(HlMesh::Faces::UP);
		//Mesh stbm(gCubeVertices, tbm.getVerticesSize(), nullptr, 0);

		//linf << memcmp(tbm.getVertices(), cube.getVertices(), cube.getVerticesSize()) << "\n";
			
		RenderGroup cubeGroup{ static_cast<const Mesh&>(tbm), DEBUG_RENDER_AMOUNT, m_ModelMatrices };

		//Chunk ch{ Position(0,0,0) };

		//ch.initialize();
		//ch.generate();
		//ch.updateRender();


		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		linf << "Done, loading texture\n";

		Texture tex{ "cobblestone.png" };

		shader.use();
		shader.setInt("texture1", 1);

		glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		linf << "Done, Generating GPUs draw commands\n";

		tbm.genDrawCommand();
		//cube.genDrawCommand();
		
		linf << "Done, entering main render loop\n";

		int currentS = 0;
		unsigned frameCount = 0;
		unsigned lastFps = 0;

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex.getId());

		while (!glfwWindowShouldClose(m_Window)) {
			float currentFrame = glfwGetTime();
			m_DeltaTime = currentFrame - m_LastFrame;
			m_LastFrame = currentFrame;
			++frameCount;

			//physicManager.simulate(m_DeltaTime);

			processInput(m_Window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			txt.startRender();
			txt.render(std::string("FPS: ")+fmt::format_int(lastFps).str(), 10.f, m_ScreenHeight - 25.f, 0.3f);
			txt.render(fmt::format("XYZ: {} / {} / {}", m_Camera->getPosition().x, m_Camera->getPosition().y, m_Camera->getPosition().z), 10.f, m_ScreenHeight-50.f, 0.3);
			txt.endRender();

			shader.use();

			glm::mat4 view = m_Camera->getViewMatrix();
			shader.setMat4("view", view);

			if ((int)(currentFrame + 1.f) > currentS) {
				currentS = currentFrame + 1.f;
				//linf << frameCount << "FPS\n";
				lastFps = frameCount;
				frameCount = 0;
			}

			//glBindVertexArray(cube.getVAO()/*Block::s_BlockMesh.getVAO()*/);
			glBindVertexArray(tbm.getVAO());

			glDrawArraysIndirect(GL_TRIANGLES, 0);
			//glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, 0);

			glfwSwapBuffers(m_Window);
			glfwPollEvents();
		}

		//delete buf;
		freeGlfw();
		return 0;
	}

	void Renderer::loadBasicAssets() {
		AssetsManager& assetManager = UnknownLegacy::getInstance().getAssetsManager();
		
		//assetManager.
	}

	inline void Renderer::freeGlfw() {
		glfwTerminate();
		m_isGlfwLoaded = false;
	}

	inline void Renderer::processInput(GLFWwindow* window) {
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

	inline void Renderer::frameBufferSizeC(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	inline void Renderer::mouseC(GLFWwindow* window, double xpos, double ypos) {
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

	inline void Renderer::scrollC(GLFWwindow* window, double xoffset, double yoffset) {
		m_Camera->processMouseScroll(yoffset);
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