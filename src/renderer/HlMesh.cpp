#include "HlMesh.h"
#include "../game/world/Chunk.h"

namespace ul {
	std::vector<HlMesh::vertex> HlMesh::cull(const Chunk& chunk, glm::vec3 pos) const {
		std::vector<vertex> nrvo;
		if (pos.x == 0) {
			auto& v = this->get(Faces::WEST);
			nrvo.insert(nrvo.end(), v.begin(), v.end());
		}
		else if (pos.x == 15) {
			auto& v = this->get(Faces::EAST);
			nrvo.insert(nrvo.end(), v.begin(), v.end());
		}
		if (pos.y == 0) {
			auto& v = this->get(Faces::UP);
			nrvo.insert(nrvo.end(), v.begin(), v.end());
		}
		else if (pos.y == 15) {
			auto& v = this->get(Faces::DOWN);
			nrvo.insert(nrvo.end(), v.begin(), v.end());
		}
		if (pos.z == 0) {
			auto& v = this->get(Faces::SOUTH);
			nrvo.insert(nrvo.end(), v.begin(), v.end());
		}
		else if (pos.z == 15) {
			auto& v = this->get(Faces::NORTH);
			nrvo.insert(nrvo.end(), v.begin(), v.end());
		}
		return nrvo;
	}
}