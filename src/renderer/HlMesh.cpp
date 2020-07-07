#include "HlMesh.h"
#include "../game/world/Chunk.h"
#include "Logger.h"

#define UL_FACE_BIT(f) (1<<(unsigned)f)
#define UL_HAS_BIT(st, f) (st & UL_FACE_BIT(f))

namespace ul {
	//Ulgly but fastest method ?
	std::pair<std::vector<HlMesh::vertex>, std::vector<unsigned>> HlMesh::cull(const Chunk& chunk, glm::vec3 pos) const {
		unsigned status = 0;
		static unsigned maxIndice = 0;
		std::pair<std::vector<vertex>, std::vector<unsigned>> nrvo;
		static auto getFace = [&nrvo, &status, this](Faces face) {
			auto v = this->get(face); //We need a copy to remove const qualifier
			nrvo.first.insert(nrvo.first.end(), v.first.begin(), v.first.end());
			for (auto& it : v.second) it += maxIndice;
			nrvo.second.insert(nrvo.second.end(), v.second.begin(), v.second.end());
			maxIndice += v.first.size();
			status |= UL_FACE_BIT(face);
		};
		
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