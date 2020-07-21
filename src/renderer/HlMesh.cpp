#include "HlMesh.h"
#include "../game/world/Chunk.h"
#include "Logger.h"

#define UL_FACE_BIT(f) (1<<(unsigned)f)
#define UL_HAS_BIT(st, f) (st & UL_FACE_BIT(f))

namespace ul {

	//Instead of chunk, we should pass a sector of chunks
	std::vector<HlMeshFace> HlMesh::cull(const Chunk& chunk, glm::vec3 pos) const {
		unsigned status = 0;
		static unsigned maxIndice = 0;
		std::vector<HlMeshFace> nrvo{  };
		static auto getFace = [&nrvo, &status, this](Faces face) {
			auto v = this->get(face); //We need a copy to remove const qualifier
			for (auto& it : v.indices) it += maxIndice;
			maxIndice += v.vertices.size();
			nrvo.emplace_back(std::move(v));
			status |= UL_FACE_BIT(face);
		};
		
		//Should be removed to check surrounding chunks
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
		
		//Ulgly but fastest method ?
		//Chunk must be replaced with another structure to check surrounding chunks
		//How to deal if surrounding chunk is not loaded ? (-> consider the block opaque
		//because this limit should never be visible)
		if (!UL_HAS_BIT(status, Faces::WEST)) {
			if (!chunk.at({ pos.x - 1, pos.y, pos.z }).getWorldObject().isOpaque()) {
				getFace(Faces::WEST);
			}
		}
		if (!UL_HAS_BIT(status, Faces::EAST)) {
			if (!chunk.at({ pos.x + 1, pos.y, pos.z }).getWorldObject().isOpaque()) {
				getFace(Faces::EAST);
			}
		}
		if (!UL_HAS_BIT(status, Faces::UP)) {
			if (!chunk.at({ pos.x, pos.y-1, pos.z }).getWorldObject().isOpaque()) {
				getFace(Faces::UP);
			}
		}
		if (!UL_HAS_BIT(status, Faces::DOWN)) {
			if (!chunk.at({ pos.x, pos.y+1, pos.z }).getWorldObject().isOpaque()) {
				getFace(Faces::DOWN);
			}
		}
		if (!UL_HAS_BIT(status, Faces::SOUTH)) {
			if (!chunk.at({ pos.x, pos.y, pos.z-1 }).getWorldObject().isOpaque()) {
				getFace(Faces::SOUTH);
			}
		}
		if (!UL_HAS_BIT(status, Faces::NORTH)) {
			if (!chunk.at({ pos.x, pos.y, pos.z+1 }).getWorldObject().isOpaque()) {
				getFace(Faces::NORTH);
			}
		}

		return nrvo;
	}
}