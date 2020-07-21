#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

namespace ul {

	class Shader {
	public:
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

		inline void use() const noexcept {
			glUseProgram(m_ProgramID);
		}

		inline void setBool(const std::string& name, bool value) const noexcept {
			glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), (int)value);
		}

		inline void setInt(const std::string& name, int value) const noexcept {
			glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), value);
		}

		inline void setFloat(const std::string& name, float value) const noexcept {
			glUniform1f(glGetUniformLocation(m_ProgramID, name.c_str()), value);
		}

		inline void setMat4(const std::string& name, const glm::mat4& mat) const noexcept {
			glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}

		inline void setVec3(const std::string& name, const glm::vec3& vec) const noexcept {
			glUniform3f(glGetUniformLocation(m_ProgramID, name.c_str()), vec.x, vec.y, vec.z);
		}

	private:
		unsigned int m_ProgramID;
	};
}