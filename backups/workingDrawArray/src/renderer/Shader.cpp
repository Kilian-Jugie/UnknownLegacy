#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace ul {

	Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		std::string vertexSrc, fragmentSrc;
		std::ifstream vShader(vertexPath), fShader(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShader.rdbuf();
		fShaderStream << fShader.rdbuf();
		vShader.close();
		fShader.close();
		vertexSrc = vShaderStream.str();
		fragmentSrc = fShaderStream.str();

		const char* vShaderCode = vertexSrc.c_str(), * fShaderCode = fragmentSrc.c_str();

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);

		int success;
		char infolog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
			std::cerr << "Vertex shader compilation failed !: " << infolog << "\n";
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
			std::cerr << "Fragment shader compilation failed !: " << infolog << "\n";
		}

		m_ProgramID = glCreateProgram();
		glAttachShader(m_ProgramID, vertexShader);
		glAttachShader(m_ProgramID, fragmentShader);
		glLinkProgram(m_ProgramID);

		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ProgramID, 512, NULL, infolog);
			std::cerr << "Shader program link failed !: " << infolog << "\n";
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::use() {
		glUseProgram(m_ProgramID);
	}

	void Shader::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), value);
	}

	void Shader::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(m_ProgramID, name.c_str()), value);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}