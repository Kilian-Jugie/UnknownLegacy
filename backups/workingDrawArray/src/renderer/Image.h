#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

namespace ul {
	class Image {
	public:
		Image(const char* path) {
			stbi_set_flip_vertically_on_load(true);
			m_Data = stbi_load(path, &m_Width, &m_Height, &m_Channels, 0);
			if (!m_Data) {
				std::cerr << "Failed to load texture !\n";
			}
		}

		int getWidth() {
			return m_Width;
		}

		int getHeight() {
			return m_Height;
		}

		int getChannels() {
			return m_Channels;
		}

		unsigned char* getData() {
			return m_Data;
		}

		void free() {
			stbi_image_free(m_Data);
		}

	private:
		int m_Width, m_Height, m_Channels;
		unsigned char* m_Data;
	};
}