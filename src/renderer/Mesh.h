#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>



namespace ul {
	class HlMeshFace;
	class Mesh {
	public:

		using buffer_t = unsigned int;

		/*Mesh(const float* vertices, GLuint verticesSize, const float* texCoords, GLuint textcoordsSize, const unsigned* indices,
			GLuint indicesSize, const unsigned* texId, GLuint texIdSize)  noexcept
			: m_Vertices{ vertices }, m_TexCoords{ texCoords }, m_TexCoordsSize{ textcoordsSize },
			m_Indices{ indices }, m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 },
			m_VerticesSize{ verticesSize }, m_IndicesSize{ indicesSize }, m_ArrayIndex{ 0 }, m_TexId{ texId }, m_TexIdSize{ texIdSize } {
			configure(); // THIS IS MANDATORY : we don't do copies of parameters arrays !!!!!!
		}*/

		Mesh(std::vector<HlMeshFace> faces) noexcept;

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

		/*inline const float* getVertices() const noexcept {
			return m_Vertices;
		}*/
		
		inline size_t getVerticesSize() const noexcept {
			return m_VerticesSize;
		}

		inline int getModelBufferOffset() const noexcept {
			return m_VerticesSize + m_TexCoordsSize + m_TexIdsSize;
		}

		void genDrawCommand();

	protected:
		virtual void configure() noexcept;

		virtual GLuint setupArrayBuffer(GLuint begin) noexcept;

		/*virtual GLuint setupModelAttribsArrays(GLuint begin) noexcept {
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
		}*/


		buffer_t m_VAO, m_VBO, m_EBO, m_TXC, m_TID, m_ArrayIndex;

		std::vector<int> m_TexIds;
		std::vector<float> m_Vertices;
		std::vector<float> m_TexCoords;
		std::vector<unsigned> m_Indices;
		GLuint m_VerticesSize, m_IndicesSize, m_TexCoordsSize, m_TexIdsSize;
	};
}