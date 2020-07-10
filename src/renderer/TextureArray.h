#pragma once
#include <filesystem>
#include <vector>

namespace ul {
	class TextureArray {
	public:
		TextureArray(unsigned width, unsigned height) : m_Width{ width }, m_Height{ height }, m_Id{ 0 }, m_TextureUnit{ 2 } {}

		inline void addFolder(std::filesystem::path path) {
			m_Paths.emplace_back(std::move(path));
		}

		unsigned getId() const noexcept {
			return m_Id;
		}

		unsigned getTextureUnit() const noexcept {
			return m_TextureUnit;
		}

		void load();
		void unload();
		void reload();

	private:
		std::vector<std::filesystem::path> m_Paths;
		unsigned m_Width, m_Height;
		unsigned m_Id, m_TextureUnit;
	};
}