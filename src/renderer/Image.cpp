#include "Image.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 26495 26451 6011 6262 6308 6387 28182 26495 4244)
#include "stb_image.h"
#pragma warning(pop)

ul::Image::Image(const char* path) noexcept {
	stbi_set_flip_vertically_on_load(true);
	m_Data = stbi_load(path, &m_Width, &m_Height, &m_Channels, 0);
	if (!m_Data) {
		std::cerr << "Failed to load texture !\n";
	}
}

void ul::Image::free() noexcept {
	stbi_image_free(m_Data);
}
