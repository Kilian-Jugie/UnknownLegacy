#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace ul {
	class Vertex {
	public:	
		template<typename _t_vec3,
			typename _t_vec2>
		Vertex(_t_vec3&& position, _t_vec2&& texcoords) noexcept :
			m_Position{ std::forward<_t_vec3>(position) },
			m_TexCoords{ std::forward<_t_vec2>(texcoords)} {
			static_assert(std::is_constructible_v<glm::vec2, _t_vec2>, "glm::vec2 must be constructible with _t_vec2");
			static_assert(std::is_constructible_v<glm::vec3, _t_vec3>, "glm::vec3 must be constructible with _t_vec3")
		}

		Vertex() = delete;
		Vertex(const Vertex&) = default;

		inline decltype(auto) getPosition() const noexcept { return m_Position; }
		inline decltype(auto) getTexCoords() const noexcept { return m_TexCoords; }


	private:
		glm::vec3 m_Position;
		glm::vec2 m_TexCoords;
	};
	
	class Mesh {
	public:
		template<typename _t_vertexarr>
		Mesh(_t_vertexarr&& vertices) : m_Vertices{std::forward<_t_vertexarr>(vertices)} {}

		decltype(auto) getVertices() const noexcept { return m_Vertices; }
	private:
		std::vector<Vertex> m_Vertices;
	};

	static const Mesh gMeshCube({
		Vertex{glm::vec3{1.f, 1.f, 1.f},glm::vec2{1.f,1.f}}
		});
}