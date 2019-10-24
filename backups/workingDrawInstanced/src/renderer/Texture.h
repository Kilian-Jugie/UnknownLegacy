#pragma once


namespace ul {
	class Texture {

	public:
		Texture(const char* textureName) noexcept; 

		const unsigned int getId() const noexcept {
			return m_Id;
		}

	private:
		unsigned int m_Id;
	};
}