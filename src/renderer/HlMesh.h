#pragma once
#include <vector>
#include <tuple>
#include <glm/vec3.hpp>
#include "HlMeshFace.h"

namespace ul {
	class Chunk;
	class HlMesh;

	class HlMesh {
	public:
		using face_t = HlMeshFace;
		using arrFaces_t = std::vector<face_t>;

		HlMesh(arrFaces_t faces) : m_Faces{ std::move(faces) }, eqVertices{  } {}

		enum class Faces : unsigned {
			NORTH,
			EAST,
			SOUTH,
			WEST,
			UP,
			DOWN
		};

		const face_t& get(Faces face) const {
			return m_Faces[(unsigned)face];
		}

		static std::pair<std::vector<float>, std::vector<float>> vertexArrayAsDualArray(const std::vector<vertex>& ver) {
			std::pair<std::vector<float>, std::vector<float>> ret;
			for (auto it : ver) {
				ret.first.insert(ret.first.end(), std::begin(it.pos), std::end(it.pos));
				ret.second.insert(ret.second.end(), std::begin(it.tex), std::end(it.tex));
			}
			return ret;
		}

		template<typename _L>void apply(_L func) {
			for (auto& it : m_Faces) {
				func(it);
			}
		}

		std::vector<HlMeshFace> cull(const Chunk& chunk, glm::vec3 pos) const;

	private:
		std::pair<std::vector<float>, std::vector<float>> eqVertices;

		arrFaces_t m_Faces;
	};
}