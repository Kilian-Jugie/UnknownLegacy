#include "Image.h"

ul::Image::Image(const char* path) noexcept {
	stbi_set_flip_vertically_on_load(true);
	m_Data = stbi_load(path, &m_Width, &m_Height, &m_Channels, 0);
	if (!m_Data) {
		std::cerr << "Failed to load texture !\n";
	}
}
