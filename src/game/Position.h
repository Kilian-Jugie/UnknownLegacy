#pragma once
#include <cstddef>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace ul {
	class Position {
	public:
		static const Position Null;

		Position(std::size_t x, std::size_t y, std::size_t z) noexcept : x(x), y(y), z(z) {}
		Position(const glm::vec3& v) noexcept : x(v.x), y(v.y), z(v.z) {}
		Position(const glm::mat4& m) noexcept : Position( m[3][0], m[3][1], m[3][2]) {}

		Position(Position&&) = default;
		Position(const Position& pos) : Position{ pos.x, pos.y, pos.z } {}

		operator glm::vec3() const noexcept {
			return glm::vec3(x, y, z);
		}

		operator glm::vec4() const noexcept {
			return glm::vec4(x, y, z, 1);
		}

		std::size_t getX() const noexcept {
			return x;
		}

		std::size_t getY() const noexcept {
			return y;
		}

		std::size_t getZ() const noexcept {
			return z;
		}

	private:
		std::size_t x, y, z;
	};

	const Position Position::Null = { 0, 0, 0 };
}