#pragma once
#include "Mesh.h"

namespace ul {
	class RenderGroup {
	public:
		RenderGroup(Mesh&& mesh, GLuint count,
			const glm::mat4* models) noexcept :
			m_Mesh{ std::move(mesh) }, m_Count{ count }, m_Models{ models }
			, m_InstanceBuffer{ 0 } {
			configure(m_Mesh.getArrayIndex());
		}

		RenderGroup(const Mesh& mesh, GLuint count,
			const glm::mat4* models) noexcept :
			m_Mesh{ mesh }, m_Count{ count }, m_Models{ models }
			, m_InstanceBuffer{ 0 } {
			configure(m_Mesh.getArrayIndex());
		}

		inline const GLuint getCount() const noexcept {
			return m_Count;
		}
	private:
		GLuint configure(GLuint arrayBegin) noexcept {
			//TODO: we should create high level structure to check execution order of mesh initialization and remove this
			glBindBuffer(GL_ARRAY_BUFFER, m_Mesh.getVBO());
			glBufferSubData(GL_ARRAY_BUFFER, m_Mesh.getModelBufferOffset(), sizeof(glm::mat4), &((*m_Models)[0]));

			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(m_Mesh.getModelBufferOffset()));
			glVertexAttribDivisor(3, 1);
			glEnableVertexAttribArray(3);

			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(m_Mesh.getModelBufferOffset() + sizeof(glm::vec4)));
			glVertexAttribDivisor(4, 1);
			glEnableVertexAttribArray(4);

			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(m_Mesh.getModelBufferOffset() + 2 * sizeof(glm::vec4)));
			glVertexAttribDivisor(5, 1);
			glEnableVertexAttribArray(5);

			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(m_Mesh.getModelBufferOffset() + 3 * sizeof(glm::vec4)));
			glVertexAttribDivisor(6, 1);
			glEnableVertexAttribArray(6);

			/*glBindVertexArray(m_Mesh.getVAO());
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
			glVertexAttribDivisor(arrayBegin, 1);*/



			return ++arrayBegin;
		}

		const Mesh& m_Mesh;
		const GLuint m_Count;
		GLuint m_InstanceBuffer;
		const glm::mat4* m_Models;
	};
}