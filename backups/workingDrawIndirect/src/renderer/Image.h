#pragma once

#define STB_IMAGE_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 26495 26451 6011 6262 6308 6387 28182 26495 4244)
#include "stb_image.h"
#pragma warning(pop)
#include <iostream>

namespace ul {
	class Image {
	public:
		Image(const char* path) noexcept {
			stbi_set_flip_vertically_on_load(true);
			m_Data = stbi_load(path, &m_Width, &m_Height, &m_Channels, 0);
			if (!m_Data) {
				std::cerr << "Failed to load texture !\n";
			}
		}

		const int getWidth() noexcept {
			return m_Width;
		}

		const int getHeight() noexcept {
			return m_Height;
		}

		const int getChannels() noexcept {
			return m_Channels;
		}

		const unsigned char* getData() noexcept {
			return m_Data;
		}

		void free() noexcept {
			stbi_image_free(m_Data);
		}

	private:
		int m_Width, m_Height, m_Channels;
		unsigned char* m_Data;
	};
}