#pragma once
#include "Mesh.h"

namespace ul {
	class RenderGroup {
	public:
		RenderGroup(Mesh&& mesh, GLuint count,
			const glm::mat4* models) noexcept :
			m_Mesh{ std::move(mesh) }, m_Count{ count }, m_Models{ models }
			, m_InstanceBuffer{ 0 } {
		}

		RenderGroup(const Mesh& mesh, GLuint count,
			const glm::mat4* models) noexcept :
			m_Mesh{ mesh }, m_Count{ count }, m_Models{ models }
			, m_InstanceBuffer{ 0 } {
		}

		inline const GLuint getCount() const noexcept {
			return m_Count;
		}

		GLuint configure(GLuint arrayBegin) noexcept;
	private:
		

		const Mesh& m_Mesh;
		const GLuint m_Count;
		GLuint m_InstanceBuffer;
		const glm::mat4* m_Models;
	};
}