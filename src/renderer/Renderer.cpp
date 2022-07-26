#include "Renderer.h"
#include "Mesh.h"
#include "RenderGroup.h"
#include "game/world/Chunk.h"
#include "ul.h"
#include "utils/StringUtils.h"
#include "TextRenderer.h"

namespace ul {
	Renderer::Renderer() : m_Window{ nullptr }, m_Camera{ nullptr }, m_TextureManager{ 16,16 } {
		m_ChunksMesh = new Mesh({});
	}

	Renderer::~Renderer() {
		if (m_isGlfwLoaded) freeGlfw();
		if (m_ChunksMesh) delete m_ChunksMesh;
		if (m_Shaders) delete m_Shaders;
		if (m_TextRenderer) delete m_TextRenderer;
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
		glfwSwapInterval(1); //VSync

		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_isGlfwLoaded = true;

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			lfat << "Failed to initialize GLAD\n";
			glfwTerminate();
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
#if GL_EXT_texture_filter_anisotropic
#define UL_ANISOTROPIC_FILTER 1
#else
#define UL_ANISOTROPIC_FILTER 0
#endif // DEBUG

		linf << "Initialization done !\n";
		return 0;
	}

	int Renderer::render(AssetsManager& assetManager, size_t shaderLocationId/*, PhysicManager& physicManager*/) {
		if (!m_isGlfwLoaded) {
			auto _inist = initGlfw();
			if (_inist != 0)
				return _inist;
		}

		m_Camera = new Camera{  };

		//linf << "Loading model matrices\n";
		//loadModelMatrices();
		linf << "Compiling shaders\n";
		
		glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CCW);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//linf << "Vertex path: " << wtos(assetManager.getPath("vertex.glsl", shaderLocationId)) << "\n";
		//linf << "Fragment path: " << wtos(assetManager.getPath("fragment.glsl", shaderLocationId)) << "\n";

		m_Shaders = new Shader(wtos(assetManager.getPath("vertex.glsl", shaderLocationId)).c_str(),
			wtos(assetManager.getPath("fragment.glsl", shaderLocationId)).c_str());
		
		
		m_TextRenderer = new TextRenderer();
		m_TextRenderer->initialize(assetManager, shaderLocationId, { m_ScreenWidth, m_ScreenHeight });
		

		//linf << "Done, filling VAO, VBO, (EBO)\n";

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//linf << "Done, loading texture\n";

		UnknownLegacy::getInstance().registerWorldObject(&Block::STONE);
		Block::AIR.setOpaque(false);
		UnknownLegacy::getInstance().registerWorldObject(&Block::AIR);
		
		Renderer::getInstance().getTextureManager().load();
		HlMeshFace::defaultTexId = static_cast<HlMeshFace::texid_t>(m_TextureManager.getTextureId("ul","error"));
		UnknownLegacy::getInstance().initializeWorldObjects();

		Chunk ch{ {0,0,0} };
		Chunk ch2{ {1,0,0} };
		//Mesh tbm = ch.toMesh();
		*m_ChunksMesh = ch.toMesh();
		*m_ChunksMesh += ch2.toMesh();

		glm::mat4x4 model{ 1.f };
		glm::translate(model, { 0,0,0 });
	
		RenderGroup cubeGroup{ static_cast<const Mesh&>(*m_ChunksMesh), 1, &model };
		//shader.setInt("texture1", 1);

		m_Shaders->use();
		glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 100.0f);
		m_Shaders->setMat4("projection", projection);

		linf << "Done, Generating GPUs draw commands\n";

		m_ChunksMesh->configure();
		cubeGroup.configure(0);

		m_ChunksMesh->genDrawCommand();
		//cube.genDrawCommand();
		
		linf << "Done, entering main render loop\n";
		
		
		loop();

		//delete buf;
		freeGlfw();
		return 0;
	}

	void Renderer::loop() {
		int currentS = 0;
		unsigned lastFps = 0;
		unsigned frameCount = 0;

		
		m_Shaders->setInt("textureUnit", m_TextureManager.getTextureUnit());

		m_TextureManager.bind();
		while (!glfwWindowShouldClose(m_Window)) {
			float currentFrame = static_cast<float>(glfwGetTime());
			m_DeltaTime = currentFrame - m_LastFrame;
			m_LastFrame = currentFrame;
			++frameCount;

			processInput(m_Window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 view = m_Camera->getViewMatrix();
			std::string cardinal{ "" };
			if (std::roundf(view[2][2]) == -1) cardinal.push_back('S');
			else if (std::roundf(view[2][2]) == 1) cardinal.push_back('N');
			if (std::roundf(view[0][2]) == -1) cardinal.push_back('W');
			else if (std::roundf(view[0][2]) == 1) cardinal.push_back('E');
			if (std::roundf(view[1][2]) == -1) cardinal.push_back('U');
			else if (std::roundf(view[1][2]) == 1) cardinal.push_back('D');


			m_TextRenderer->startRender();
			m_TextRenderer->render(std::string("FPS: ") + std::to_string(lastFps), 10.f, m_ScreenHeight - 25.f, 0.3f);
			m_TextRenderer->render(std::format("XYZ: {} / {} / {}", m_Camera->getPosition().x, m_Camera->getPosition().y, m_Camera->getPosition().z), 10.f, m_ScreenHeight - 50.f, 0.3);
			m_TextRenderer->render(std::format("Vertices: {}", m_ChunksMesh->getVerticesSize() / sizeof(float)), 10.f, m_ScreenHeight - 75.f, 0.3f);
			m_TextRenderer->render(std::format("Watching: XYZ: {} / {} / {} Cardinal: {}", view[0][2], view[1][2], view[2][2], cardinal), 10.f, m_ScreenHeight - 100.f, 0.3f);
			m_TextRenderer->endRender();

			m_Shaders->use();
			m_Shaders->setMat4("view", view);

			if ((int)(currentFrame + 1.f) > currentS) {
				currentS = static_cast<int>(currentFrame + 1.f);
				//linf << frameCount << "FPS\n";
				lastFps = frameCount;
				frameCount = 0;
			}

			glBindVertexArray(m_ChunksMesh->getVAO());

			//glBindVertexArray(cube.getVAO()/*Block::s_BlockMesh.getVAO()*/);
			

			//glDrawArraysIndirect(GL_TRIANGLES, 0);
			glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(m_Window);
			glfwPollEvents();
		}
	}

	void Renderer::loadBasicAssets() {
	}

	inline void Renderer::freeGlfw() {
		glfwTerminate();
		m_isGlfwLoaded = false;
	}

	inline void Renderer::processInput(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera->processKeyboard(CameraMovement::FORWARD, m_DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera->processKeyboard(CameraMovement::BACKWARD, m_DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera->processKeyboard(CameraMovement::LEFT, m_DeltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera->processKeyboard(CameraMovement::RIGHT, m_DeltaTime);
	}

	inline void Renderer::frameBufferSizeC(int width, int height) {
		glViewport(0, 0, width, height);
	}

	inline void Renderer::mouseC(double xpos, double ypos) {
		if (firstMouse) {
			m_MouseLastX = static_cast<float>(xpos);
			m_MouseLastY = static_cast<float>(ypos);
			firstMouse = false;
		}

		float xoffset = static_cast<float>(xpos) - m_MouseLastX;
		float yoffset = m_MouseLastY - static_cast<float>(ypos); // reversed since y-coordinates go from bottom to top
		m_MouseLastX = static_cast<float>(xpos);
		m_MouseLastY = static_cast<float>(ypos);

		m_Camera->processMouseMovement(xoffset, yoffset);
	}

	inline void Renderer::scrollC(double yoffset) {
		m_Camera->processMouseScroll(static_cast<float>(yoffset));
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		_CRT_UNUSED(window);
		Renderer::getInstance().frameBufferSizeC(width, height);
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		_CRT_UNUSED(window);
		Renderer::getInstance().mouseC(xpos, ypos);
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		_CRT_UNUSED(window);
		_CRT_UNUSED(xoffset);
		Renderer::getInstance().scrollC(yoffset);
	}

	void logMat4(glm::mat4 model) {
		linf << "[" << model[0][0] << ", " << model[0][1] << ", " << model[0][2] << ", " << model[0][3] << "\n"
			<< model[1][0] << ", " << model[1][1] << ", " << model[1][2] << ", " << model[1][3] << "\n"
			<< model[2][0] << ", " << model[2][1] << ", " << model[2][2] << ", " << model[2][3] << "\n"
			<< model[3][0] << ", " << model[3][1] << ", " << model[3][2] << ", " << model[3][3] << "]\n";
	}
}