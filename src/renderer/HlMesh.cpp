#include "HlMesh.h"
#include "../game/world/Chunk.h"
#include "Logger.h"

#define UL_FACE_BIT(f) (1<<(unsigned)f)
#define UL_HAS_BIT(st, f) (st & UL_FACE_BIT(f))

namespace ul {
	//Ulgly but fastest method ?
	std::vector<HlMesh::vertex> HlMesh::cull(const Chunk& chunk, glm::vec3 pos) const {
		unsigned status = 0;
		std::vector<vertex> nrvo;
		static auto getFace = [&nrvo, &status, this](Faces face) {
			auto& v = this->get(face);
			nrvo.insert(nrvo.end(), v.begin(), v.end());
			status |= UL_FACE_BIT(face);
		};

		if (pos.x == 10 && pos.y == 10 && pos.z == 10) {
			linf << "tobeculled\n";
		}
		
		if (pos.x == 0) {
			getFace(Faces::WEST);
		}
		else if (pos.x == Chunk::CHUNK_SIZE_X-1) {
			getFace(Faces::EAST);
		}
		if (pos.y == 0) {
			getFace(Faces::UP);
		}
		else if (pos.y == Chunk::CHUNK_SIZE_Y - 1) {
			getFace(Faces::DOWN);
		}
		if (pos.z == 0) {
			getFace(Faces::SOUTH);
		}
		else if (pos.z == Chunk::CHUNK_SIZE_Z - 1) {
			getFace(Faces::NORTH);
		}
		
		if (!UL_HAS_BIT(status, Faces::WEST)) {
			if (!chunk.at({ pos.x - 1, pos.y, pos.z }).obj.isOpaque()) {
				getFace(Faces::WEST);
			}
		}
		if (!UL_HAS_BIT(status, Faces::EAST)) {
			if (!chunk.at({ pos.x + 1, pos.y, pos.z }).obj.isOpaque()) {
				getFace(Faces::EAST);
			}
		}
		if (!UL_HAS_BIT(status, Faces::UP)) {
			if (!chunk.at({ pos.x, pos.y-1, pos.z }).obj.isOpaque()) {
				getFace(Faces::UP);
			}
		}
		if (!UL_HAS_BIT(status, Faces::DOWN)) {
			if (!chunk.at({ pos.x, pos.y+1, pos.z }).obj.isOpaque()) {
				getFace(Faces::DOWN);
			}
		}
		if (!UL_HAS_BIT(status, Faces::SOUTH)) {
			if (!chunk.at({ pos.x, pos.y, pos.z-1 }).obj.isOpaque()) {
				getFace(Faces::SOUTH);
			}
		}
		if (!UL_HAS_BIT(status, Faces::NORTH)) {
			if (!chunk.at({ pos.x, pos.y, pos.z+1 }).obj.isOpaque()) {
				getFace(Faces::NORTH);
			}
		}

		return nrvo;
	}
}