#include "TextureManager.h"
#include "Image.h"
#include "StringUtils.h"
#include "glad/glad.h"
#include "Logger.h"
#include "../maths/Maths.h"

namespace ul {

	void TextureManager::bind() const noexcept {
		glBindTextureUnit(getTextureUnit(), getId());
	}

	void TextureManager::load() {
		std::vector<Image> tmpData;
		try {
			for (auto& dir : m_Paths) {
				for (auto& it : std::filesystem::recursive_directory_iterator(dir.path)) {
					if (it.is_regular_file()) {
						linf << "Loading texture " << wtos(it.path()) << "\n";
						tmpData.push_back({ wtos(it.path()).c_str() });
						auto& file = it.path().filename().string();
						m_Textures.push_back({file.substr(0,file.find_last_of('.')) , tmpData.size() - 1, dir });

					}
				}
			}
		}
		catch (std::exception& e) {
			lerr << e.what() << "\n";
		}

		glGenTextures(1, &m_Id);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_Id);
		float aniso = 0.f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
		glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, log2_64(std::min(m_Width, m_Height)), GL_RGBA8, m_Width, m_Height, tmpData.size());

		auto layer = 0;
		for (auto& it : tmpData) {
 			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, m_Width, m_Height, 1, GL_RGBA, GL_UNSIGNED_BYTE, it.getData());
			++layer;
		}
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);	
	}
}