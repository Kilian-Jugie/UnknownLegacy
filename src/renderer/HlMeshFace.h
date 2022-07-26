#pragma once
#include <vector>

namespace ul {
	struct vertex {
		float pos[3];
		float tex[2];
	};

	struct HlMeshFace {
		using vertices_t = std::vector<vertex>;
		using indices_t = std::vector<unsigned>;
		using texid_t = unsigned;

		vertices_t vertices;
		indices_t indices;
		texid_t texId;

		static texid_t defaultTexId;

		HlMeshFace(vertices_t v, indices_t i) : vertices{ v }, indices{ i }, texId{ defaultTexId } {}
		HlMeshFace(HlMeshFace&&) = default;
		HlMeshFace(const HlMeshFace&) = default;

		HlMeshFace& operator=(const HlMeshFace&) = default;
	};
}