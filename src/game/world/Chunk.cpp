#include "Chunk.h"
#include "block/Block.h"

void ul::Chunk::initBlocks() {
	for (int x(0); x < CHUNK_SIZE_X; ++x) {
		for (int y(0); y < CHUNK_SIZE_Y; ++y) {
			for (int z(0); z < CHUNK_SIZE_Z; ++z) {
				if (x == 10 && y == 10 && z == 10) {

					blocks[x][y][z] = new InstancedObject(glm::mat4(), Blocks::AIR);
					glm::translate(blocks[x][y][z]->getModel(), { x,y,z });
					glm::scale(blocks[x][y][z]->getModel(), { 0.5f, 0.5f, 0.5f });
				}
				else {
					blocks[x][y][z] = new InstancedObject(glm::mat4(), Blocks::STONE);
					glm::translate(blocks[x][y][z]->getModel(), { x,y,z });
					glm::scale(blocks[x][y][z]->getModel(), { 0.5f, 0.5f, 0.5f });
				}
			}
		}
	}
}

ul::Mesh ul::Chunk::toMesh() {
	std::vector<HlMeshFace> faces;
	for (int x(0); x < CHUNK_SIZE_X; ++x) {
		for (int y(0); y < CHUNK_SIZE_Y; ++y) {
			for (int z(0); z < CHUNK_SIZE_Z; ++z) {

				/*if (x == 0 && y == 0 && z == 0) { //Testing purpose
					blockObject.getMesh().apply([](HlMesh::face_t& face) {
						face.texId = 2;
						});
				}*/

				auto tmpFaces = blocks[x][y][z]->getWorldObject().getMesh().cull(*this, { x,y,z });
				if (!tmpFaces.empty()) {
					float tr[] = { x,y,z };
					for (auto& it : tmpFaces) it.vertices = translate(it.vertices, tr);
					faces.insert(faces.end(), tmpFaces.begin(), tmpFaces.end());
				}

				/*if (x == 0 && y == 0 && z == 0) {
					blockObject.getMesh().apply([](HlMesh::face_t& face) {
						face.texId = HlMesh::face_t::defaultTexId;
						});
				}*/
			}
		}
	}
	return Mesh(faces);
}
