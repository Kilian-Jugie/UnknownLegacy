#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "HlMeshFace.h"

namespace ul {
	class HlMeshFace;

	class Mesh {
	public:
		using buffer_t = unsigned int;

		Mesh(std::vector<HlMeshFace> faces) noexcept;
		Mesh(const Mesh&) = default;

		Mesh& operator+=(const Mesh& mesh) noexcept;
		Mesh& operator=(const Mesh&) = default;


		~Mesh() {
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
		
		inline size_t getVerticesSize() const noexcept {
			return m_VerticesSize;
		}

		inline int getModelBufferOffset() const noexcept {
			return m_VerticesSize + m_TexCoordsSize + m_TexIdsSize;
		}

		bool isConfigured() const noexcept;

		void genDrawCommand();

		virtual void configure() noexcept;

	protected:
		virtual GLuint setupArrayBuffer(GLuint begin) noexcept;

		buffer_t m_VAO, m_VBO, m_EBO, m_TXC, m_TID, m_ArrayIndex;

		std::vector<int> m_TexIds;
		std::vector<float> m_Vertices;
		std::vector<float> m_TexCoords;
		std::vector<unsigned> m_Indices;
		GLuint m_VerticesSize, m_IndicesSize, m_TexCoordsSize, m_TexIdsSize;
	};
}