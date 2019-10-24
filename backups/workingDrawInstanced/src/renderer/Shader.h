#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

namespace ul {

	class Shader {
	public:
		unsigned int m_ProgramID;

		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

		void use();

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
	};
}