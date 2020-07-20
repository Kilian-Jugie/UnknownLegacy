#pragma once
#include "Mesh.h"

namespace ul {
	class RenderGroup {
	public:
		RenderGroup(Mesh&& mesh, GLuint count,
			const glm::mat4* models) noexcept :
			m_Mesh{ std::move(mesh) }, m_Count{ count }, m_Models{ models }
			, m_InstanceBuffer{ 0 } {
			//configure(m_Mesh.getArrayIndex());
		}

		RenderGroup(const Mesh& mesh, GLuint count,
			const glm::mat4* models) noexcept :
			m_Mesh{ mesh }, m_Count{ count }, m_Models{ models }
			, m_InstanceBuffer{ 0 } {
			//configure(m_Mesh.getArrayIndex());
		}

		inline const GLuint getCount() const noexcept {
			return m_Count;
		}
	private:
		GLuint configure(GLuint arrayBegin) noexcept {
			glBindVertexArray(m_Mesh.getVAO());
			glGenBuffers(1, &m_InstanceBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer);
			glBufferData(GL_ARRAY_BUFFER, m_Count * sizeof(glm::mat4), &((*m_Models)[0]), GL_STATIC_DRAW);
			glEnableVertexAttribArray(arrayBegin);
			glVertexAttribPointer(arrayBegin, 4, GL_FLOAT, GL_FALSE,sizeof(glm::mat4), (void*)0);
			glVertexAttribDivisor(arrayBegin, 1);

			glEnableVertexAttribArray(++arrayBegin);
			glVertexAttribPointer(arrayBegin, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glVertexAttribDivisor(arrayBegin, 1);

			glEnableVertexAttribArray(++arrayBegin);
			glVertexAttribPointer(arrayBegin, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glVertexAttribDivisor(arrayBegin, 1);

			glEnableVertexAttribArray(++arrayBegin);
			glVertexAttribPointer(arrayBegin, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
			glVertexAttribDivisor(arrayBegin, 1);

			return ++arrayBegin;
		}

		const Mesh& m_Mesh;
		const GLuint m_Count;
		GLuint m_InstanceBuffer;
		const glm::mat4* m_Models;
	};
}