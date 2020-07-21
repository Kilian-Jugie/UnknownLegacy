#pragma once
#include "../Position.h"
#include "block/Block.h"
#include <vector>
#include "../renderer/Mesh.h"
#include "../game/world/InstancedObject.h"


namespace ul {

	class Chunk {
	public:
		static constexpr size_t CHUNK_SIZE_X = 16;
		static constexpr size_t CHUNK_SIZE_Z = 16;
		static constexpr size_t CHUNK_SIZE_Y = 256;

		Chunk(const Position& pos) : m_Position{ pos } {
			initBlocks();
		}

		~Chunk() {
			deleteBlocks();
		}

		void initBlocks();

		void deleteBlocks() {
			for (int x(0); x < CHUNK_SIZE_X; ++x) {
				for (int y(0); y < CHUNK_SIZE_Y; ++y) {
					for (int z(0); z < CHUNK_SIZE_Z; ++z) {
						delete blocks[x][y][z];
					}
				}
			}
		}

		std::vector<vertex>& translate(std::vector<vertex>& vec, float translation[3]) {
			for (auto& it : vec) {
				for (size_t i(0); i < 3; ++i) {
					it.pos[i] += translation[i];
				}
			}
			return vec;
		}

		const InstancedObject& at(glm::ivec3 pos) const {
			return *blocks[pos.x][pos.y][pos.z];
		}

		Mesh toMesh();

	private:
		const Position m_Position;
		InstancedObject* blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	};
}