#include "Mesh.h"
#include "Renderer.h"
#include "HlMesh.h"

namespace ul {
	

	Mesh::Mesh(std::vector<HlMeshFace> faces) noexcept : m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 }, m_ArrayIndex{ 0 }  {
		for (auto& it : faces) {
			auto dualVertex = HlMesh::vertexArrayAsDualArray(it.vertices);
			m_Vertices.insert(m_Vertices.end(), dualVertex.first.begin(), dualVertex.first.end());
			m_TexCoords.insert(m_TexCoords.end(), dualVertex.second.begin(), dualVertex.second.end());
			m_Indices.insert(m_Indices.end(), it.indices.begin(), it.indices.end());
			//Rendering is working with vertex and not faces and we know that we have only 1 texture id by face
			//but opengl doesn't know what is a face so we must provide "an id by vertex".
			//TODO look for some fixes in the renderer (array dividor ?) to optimize memory here

			//a vertex = 3 pos, vertices are appened together in the array, we want 1 id/vertex so vert number = total poses / 3
			for(int i(0); i<dualVertex.first.size()/3; ++i)
				m_TexIds.push_back(it.texId);
		}
		m_VerticesSize = m_Vertices.size() * sizeof(float);
		m_TexCoordsSize = m_TexCoords.size() * sizeof(float);
		m_IndicesSize = m_Indices.size() * sizeof(unsigned);
		m_TexIdsSize = m_TexIds.size() * sizeof(int);

		configure();
	}
	
	void Mesh::genDrawCommand() {
		Renderer::IndirectDrawCmd testDraw;
		testDraw.vertexCount = m_VerticesSize / 5; //Calculation to be confirmed
		testDraw.instanceCount = DEBUG_RENDER_AMOUNT;
		testDraw.firstIndex = 0;
		testDraw.baseInstance = 0;
		testDraw.baseVertex = 0;

		unsigned indirectBuffer;

		glGenBuffers(1, &indirectBuffer);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(testDraw), &testDraw, GL_STATIC_DRAW);
	}
	
	void Mesh::configure() noexcept {
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndicesSize, m_Indices.data(), GL_STATIC_DRAW);

		m_ArrayIndex = setupArrayBuffer(0);
	}
	
	GLuint Mesh::setupArrayBuffer(GLuint begin) noexcept {
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		/*glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3((float)0, 0.0f, (float)0));
		model = glm::scale(model, glm::vec3(0.5f));*/

		glBufferData(GL_ARRAY_BUFFER, m_VerticesSize + m_TexCoordsSize + m_TexIdsSize + sizeof(glm::mat4), 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_VerticesSize, m_Vertices.data());
		glBufferSubData(GL_ARRAY_BUFFER, m_VerticesSize, m_TexCoordsSize, m_TexCoords.data());
		glBufferSubData(GL_ARRAY_BUFFER, m_VerticesSize + m_TexCoordsSize, m_TexIdsSize, m_TexIds.data());
		

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(m_VerticesSize));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(m_VerticesSize+m_TexCoordsSize));
		glEnableVertexAttribArray(2);

		

		return ++begin;
	}
}