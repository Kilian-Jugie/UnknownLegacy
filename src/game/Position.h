#pragma once
#include <cstddef>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace ul {
	class Position {
	public:
		static const Position Null;

		Position(std::size_t x, std::size_t y, std::size_t z) noexcept : x(x), y(y), z(z) {}
		Position(const glm::vec3& v) noexcept : x(static_cast<std::size_t>(v.x)), 
			y(static_cast<std::size_t>(v.y)), z(static_cast<std::size_t>(v.z)) {}
		Position(const glm::mat4& m) noexcept : Position(static_cast<std::size_t>(m[3][0]),
			static_cast<std::size_t>(m[3][1]), static_cast<std::size_t>(m[3][2])) {}

		Position(Position&&) = default;
		Position(const Position& pos) : Position{ pos.x, pos.y, pos.z } {}

		std::size_t operator[](std::size_t p) {
			switch (p) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: return static_cast<size_t>(-1);
			}
		}

		inline operator glm::vec3() const noexcept {
			return glm::vec3(x, y, z);
		}

		inline operator glm::vec4() const noexcept {
			return glm::vec4(x, y, z, 1);
		}

		inline std::size_t getX() const noexcept {
			return x;
		}

		inline std::size_t getY() const noexcept {
			return y;
		}

		inline std::size_t getZ() const noexcept {
			return z;
		}

	private:
		std::size_t x, y, z;
	};

	
}