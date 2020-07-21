#include "RenderGroup.h"

GLuint ul::RenderGroup::configure(GLuint arrayBegin) noexcept {
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
	return ++arrayBegin;
}
