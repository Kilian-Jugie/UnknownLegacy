#pragma once
#include "../Position.h"
#include "block/Block.h"
#include <vector>
#include "../renderer/Mesh.h"
#include "../game/world/InstancedObject.h"
#include <glm/mat4x4.hpp>


namespace ul {

	class Chunk {
	public:
		static constexpr std::size_t CHUNK_SIZE[] = { 16,256,16 };
		static constexpr size_t CHUNK_SIZE_X = CHUNK_SIZE[0];
		static constexpr size_t CHUNK_SIZE_Z = CHUNK_SIZE[2];
		static constexpr size_t CHUNK_SIZE_Y = CHUNK_SIZE[1];


		Chunk(const Position& pos) : m_Position{ pos }, m_Model{ 1.f } {
			glm::translate(m_Model, (glm::vec3)m_Position);
			initBlocks();
		}

		~Chunk() {
			deleteBlocks();
		}

		void initBlocks();

		void deleteBlocks();

		std::vector<vertex>& translate(std::vector<vertex>& vec, float local[3], Position abs) {
			for (auto& it : vec) {
				for (size_t i(0); i < 3; ++i) {
					it.pos[i] += local[i] + abs[i]*CHUNK_SIZE[i];
				}
			}
			return vec;
		}

		const InstancedObject& at(glm::ivec3 pos) const {
			return *blocks[pos.x][pos.y][pos.z];
		}

		Mesh toMesh();

		inline glm::mat4x4 getModel() const noexcept {
			return m_Model;
		}

	private:
		const Position m_Position;
		glm::mat4 m_Model;
		//We can't use heap array because it cause a stack overflow when using multiple chunks
		std::vector<std::vector<std::vector<InstancedObject*>>> blocks;
		//InstancedObject* blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
	};
}