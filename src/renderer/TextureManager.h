#pragma once
#include <filesystem>
#include <vector>
#include <numeric>

namespace ul {
	class TextureManager {
		static inline unsigned ID_INDEX = 0;
		static inline unsigned UNIT_INDEX = 0;

	public:
		struct TextureLocationPath {
			std::filesystem::path path;
			std::string domain;
		};

		struct Texture {
			std::string name;
			std::size_t id;
			TextureLocationPath& path;

			static std::size_t ID_INCORRECT;
		};

		inline TextureManager(unsigned width, unsigned height) : m_Width{ width }, m_Height{ height }, m_Id{ ID_INDEX++ }, m_TextureUnit{ UNIT_INDEX++ } {}


		void bind() const noexcept;

		inline void addFolder(std::string domain, std::filesystem::path path) {
			m_Paths.emplace_back(std::move(TextureLocationPath{ std::move(path),std::move(domain) }));
		}

		unsigned getId() const noexcept {
			return m_Id;
		}

		unsigned getTextureUnit() const noexcept {
			return m_TextureUnit;
		}

		std::size_t getTextureId(std::string domain, std::string name) {
			for (auto& it : m_Textures) {
				if (it.path.domain == domain && it.name == name) return it.id;
			}
			return Texture::ID_INCORRECT;
		}

		void load();
		void unload();
		void reload();

	private:
		std::vector<TextureLocationPath> m_Paths;
		std::vector<Texture> m_Textures;
		unsigned m_Width, m_Height;
		unsigned m_Id, m_TextureUnit;
	};

	
	
}