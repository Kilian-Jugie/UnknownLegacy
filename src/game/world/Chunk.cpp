#include "Chunk.h"
#include "block/Block.h"

void ul::Chunk::initBlocks() {
	for (int x(0); x < CHUNK_SIZE_X; ++x) {
		blocks.push_back({});
		for (int y(0); y < CHUNK_SIZE_Y; ++y) {
			blocks[x].push_back({});
			for (int z(0); z < CHUNK_SIZE_Z; ++z) {
				
				if (x == 10 && y == 10 && z == 10) {
					blocks[x][y].push_back(new InstancedObject(glm::mat4(), Blocks::AIR));
					//blocks[x][y][z] = 
					glm::translate(blocks[x][y][z]->getModel(), { x,y,z });
					glm::scale(blocks[x][y][z]->getModel(), { 0.5f, 0.5f, 0.5f });
				}
				else {
					//blocks[x][y][z] = ;
					blocks[x][y].push_back(new InstancedObject(glm::mat4(), Blocks::STONE));
					glm::translate(blocks[x][y][z]->getModel(), { x,y,z });
					glm::scale(blocks[x][y][z]->getModel(), { 0.5f, 0.5f, 0.5f });
				}
			}
		}
	}
}

void ul::Chunk::deleteBlocks() {
	for (int x(0); x < CHUNK_SIZE_X; ++x) {
		for (int y(0); y < CHUNK_SIZE_Y; ++y) {
			for (int z(0); z < CHUNK_SIZE_Z; ++z) {
				delete blocks[x][y][z];
			}
		}
	}
}

ul::Mesh ul::Chunk::toMesh() {
	std::vector<HlMeshFace> faces;
	for (int x(0); x < CHUNK_SIZE_X; ++x) {
		for (int y(0); y < CHUNK_SIZE_Y; ++y) {
			for (int z(0); z < CHUNK_SIZE_Z; ++z) {
				auto tmpFaces = blocks[x][y][z]->getWorldObject().getMesh().cull(*this, { x,y,z });
				if (!tmpFaces.empty()) {
					float tr[] = { x,y,z };
					for (auto& it : tmpFaces) it.vertices = translate(it.vertices, tr, m_Position);
					faces.insert(faces.end(), tmpFaces.begin(), tmpFaces.end());
				}
			}
		}
	}
	return Mesh(faces);
}
