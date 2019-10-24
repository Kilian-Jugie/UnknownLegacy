#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

namespace ul {
	class Mesh {
	public:

		using buffer_t = unsigned int;

		Mesh(const float* vertices, GLuint verticesSize, const GLushort* indices,
			GLuint indicesSize)  noexcept
			: m_Vertices{ vertices }, m_Indices{ indices },
			m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 },
			m_VerticesSize{ verticesSize }, m_IndicesSize{ indicesSize }, m_ArrayIndex{ 0 } {
			configure();
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
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

			glEnableVertexAttribArray(++begin);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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


		buffer_t m_VAO, m_VBO, m_EBO, m_ArrayIndex;

		const float* m_Vertices;
		const GLushort* m_Indices;
		const GLuint m_VerticesSize, m_IndicesSize;
	};
}