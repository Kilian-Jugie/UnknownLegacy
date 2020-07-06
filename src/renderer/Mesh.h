#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


namespace ul {
	class Mesh {
	public:

		using buffer_t = unsigned int;

		Mesh(const float* vertices, GLuint verticesSize, const float* texCoords, GLuint textcoordsSize, const GLushort* indices,
			GLuint indicesSize)  noexcept
			: m_Vertices{ vertices }, m_TexCoords{ texCoords }, m_TexCoordsSize{ textcoordsSize },
			m_Indices{ indices }, m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 },
			m_VerticesSize{ verticesSize }, m_IndicesSize{ indicesSize }, m_ArrayIndex{ 0 } {
			configure();
		}

		~Mesh() {
			//glDeleteBuffers(4, {})
		}

		inline buffer_t getVAO() const noexcept {
			return m_VAO;
		}

		inline buffer_t getVBO() const noexcept {
			return m_VBO;
		}

		inline buffer_t getEBO() const noexcept {
			return m_EBO;
		}

		inline buffer_t getArrayIndex() const noexcept {
			return m_ArrayIndex;
		}

		inline const float* getVertices() const noexcept {
			return m_Vertices;
		}
		
		inline size_t getVerticesSize() const noexcept {
			return m_VerticesSize;
		}

		void genDrawCommand();

	protected:
		virtual void configure() noexcept {
			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);

			glGenBuffers(1, &m_VBO);
			glGenBuffers(1, &m_EBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndicesSize, m_Indices, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, m_VerticesSize, m_Vertices, GL_STATIC_DRAW);

			m_ArrayIndex = setupVertexAttribsArrays(0);



			//glBindBuffer(GL_ARRAY_BUFFER, m_ObjBuffer);
			//glBufferData(GL_ARRAY_BUFFER, m_ModelsCount * sizeof(glm::mat4), &((*m_Models)[0]), GL_STATIC_DRAW);

			//setupModelAttribsArrays(e);

		}

		virtual GLuint setupVertexAttribsArrays(GLuint begin) noexcept {
			glEnableVertexAttribArray(begin);
			glVertexAttribPointer(begin, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			glGenBuffers(1, &m_TXC);
			glBindBuffer(GL_ARRAY_BUFFER, m_TXC);
			glBufferData(GL_ARRAY_BUFFER, m_TexCoordsSize, m_TexCoords, GL_STATIC_DRAW);

			glEnableVertexAttribArray(++begin);
			glVertexAttribPointer(begin, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			return ++begin;
		}

		virtual GLuint setupModelAttribsArrays(GLuint begin) noexcept {
			glEnableVertexAttribArray(begin);
			glVertexAttribPointer(begin, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glVertexAttribDivisor(begin, 1);

			glEnableVertexAttribArray(++begin);
			glVertexAttribPointer(begin, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glVertexAttribDivisor(begin, 1);

			glEnableVertexAttribArray(++begin);
			glVertexAttribPointer(begin, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glVertexAttribDivisor(begin, 1);

			glEnableVertexAttribArray(++begin);
			glVertexAttribPointer(begin, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
			glVertexAttribDivisor(begin, 1);

			return ++begin;
		}


		buffer_t m_VAO, m_VBO, m_EBO, m_TXC, m_ArrayIndex;

		const float* m_Vertices;
		const float* m_TexCoords;
		const GLushort* m_Indices;
		const GLuint m_VerticesSize, m_IndicesSize, m_TexCoordsSize;
	};
}