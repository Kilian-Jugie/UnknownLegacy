#pragma once

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

		void free() noexcept;
	private:
		int m_Width, m_Height, m_Channels;
		unsigned char* m_Data;
	};
}