#pragma once
#include <glm/glm.hpp>
#include <map>
#include "Shader.h"
#include "../filesystem/AssetsManager.h"

namespace ul {
	class Shader;

	class TextRenderer {
	public:
		struct Character {
			unsigned int TextureID; // ID handle of the glyph texture
			glm::ivec2   Size;      // Size of glyph
			glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
			unsigned int Advance;   // Horizontal offset to advance to next glyph
		};
		std::map<char, Character> Characters;


		void initialize(AssetsManager& manager, size_t shaderLocId, glm::vec2 screen);
		void clean();

		void startRender(glm::vec3 color = { 1.f,1.f,1.f });
		void render(std::string text, float x, float y, float scale);
		void endRender();

	private:
		Shader* m_Shader;
		unsigned m_VAO, m_VBO;
	};
}