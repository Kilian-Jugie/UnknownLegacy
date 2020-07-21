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
		Image(const char* path) noexcept;

		inline const int getWidth() const noexcept {
			return m_Width;
		}

		inline const int getHeight() const noexcept {
			return m_Height;
		}

		inline const int getChannels() const noexcept {
			return m_Channels;
		}

		inline const unsigned char* getData() const noexcept {
			return m_Data;
		}

		inline void free() noexcept {
			stbi_image_free(m_Data);
		}

	private:
		int m_Width, m_Height, m_Channels;
		unsigned char* m_Data;
	};
}