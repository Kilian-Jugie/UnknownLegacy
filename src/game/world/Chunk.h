#pragma once
#include <Types.h>
#include "../Position.h"
#include "block/Block.h"
//#include "block/BlockInstance.h"
#include "Constants.h"
#include <vector>
#include <functional>
#include "../renderer/RenderGroup.h"
#include "../renderer/StaticVertices.h"
#include "../renderer/HlMesh.h"
#include "../game/world/InstancedObject.h"


namespace ul {

	class Chunk {
	public:
		static constexpr size_t CHUNK_SIZE_X = 16;
		static constexpr size_t CHUNK_SIZE_Z = 16;
		static constexpr size_t CHUNK_SIZE_Y = 256;

		Chunk(const Position& pos) : m_Position{ pos }, block{"block", gBlockHlMesh}, air("air", gBlockHlMesh, false) {
			initBlocks();
		}

		~Chunk() {
			deleteBlocks();
		}

		void initBlocks() {
			for (int x(0); x < CHUNK_SIZE_X; ++x) {
				for (int y(0); y < CHUNK_SIZE_Y; ++y) {
					for (int z(0); z < CHUNK_SIZE_Z; ++z) {
						if (x == 10 && y == 10 && z == 10) {
							
							blocks[x][y][z] = new InstancedObject(glm::mat4(), air);
							glm::translate(blocks[x][y][z]->model, { x,y,z });
							glm::scale(blocks[x][y][z]->model, { 0.5f, 0.5f, 0.5f });
						}
						else {
							blocks[x][y][z] = new InstancedObject(glm::mat4(), block);
							glm::translate(blocks[x][y][z]->model, { x,y,z });
							glm::scale(blocks[x][y][z]->model, { 0.5f, 0.5f, 0.5f });
						}
					}
				}
			}
		}

		void deleteBlocks() {
			for (int x(0); x < CHUNK_SIZE_X; ++x) {
				for (int y(0); y < CHUNK_SIZE_Y; ++y) {
					for (int z(0); z < CHUNK_SIZE_Z; ++z) {
						delete blocks[x][y][z];
					}
				}
			}
		}

		/*std::vector<glm::mat4> getModels() {
			std::vector<glm::mat4> ret;
			for (int x(0); x < 16; ++x) {
				for (int y(0); y < 16; ++y) {
					for (int z(0); z < 16; ++z) {
						ret.push_back(blocks[x][y][z]->model);
					}
				}
			}
			return ret;
		}*/

		std::vector<HlMesh::vertex>& translate(std::vector<HlMesh::vertex>& vec, float translation[3]) {
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

		Mesh toMesh(HlMesh* blockMesh) {
			std::vector<HlMesh::vertex> vertices;
			std::vector<unsigned> indices;
			std::pair<std::vector<HlMesh::vertex>, std::vector<unsigned>> tmpV;
			for (int x(0); x < CHUNK_SIZE_X; ++x) {
				for (int y(0); y < CHUNK_SIZE_Y; ++y) {
					for (int z(0); z < CHUNK_SIZE_Z; ++z) {
						tmpV = block.getMesh().cull(*this, { x,y,z });
						if (!tmpV.first.empty()) {
							float tr[] = { x,y,z };
							tmpV.first = translate(tmpV.first, tr);
							vertices.insert(vertices.end(), tmpV.first.begin(), tmpV.first.end());
							indices.insert(indices.end(), tmpV.second.begin(), tmpV.second.end());
						}
					}
				}
			}
			auto vert = HlMesh::vertexArrayAsDualArray(vertices);
			return Mesh(vert.first.data(), vert.first.size() * sizeof(float), vert.second.data(), vert.second.size()*sizeof(float), indices.data(), indices.size()*sizeof(unsigned));
		}

	private:
		const Position m_Position;
		InstancedObject* blocks[CHUNK_SIZE_X][CHUNK_SIZE_Y][CHUNK_SIZE_Z];
		WorldObject block;
		WorldObject air;
	};


	/*class Chunk {
	public:

		Chunk(const Position& pos) noexcept : m_Position(pos), m_Id{ ++s_CurrentId }, m_Render{ nullptr } {}

		BlockInstance& operator()(size_t x, size_t y, size_t z) noexcept {
			return *m_Blocks[x][y][z];
		}

		void initialize() noexcept {
			m_Blocks.resize(UL_CHUNK_SIZE);
			for (std::size_t x(0); x < UL_CHUNK_SIZE; ++x) {
				m_Blocks[x].resize(UL_CHUNK_SIZE);
				for (std::size_t y(0); y < UL_CHUNK_SIZE; ++y) {
					m_Blocks[x][y].resize(UL_MAX_HEIGHT);
				}
			}

		}

		void generate() noexcept {
			for (std::size_t x(0); x < UL_CHUNK_SIZE; ++x) {
				for (std::size_t y(0); y < UL_CHUNK_SIZE; ++y) {
					for (std::size_t z(0); z < UL_MAX_HEIGHT; ++z) {
						m_Blocks[x][y][z] = std::move(Blocks::TEST_BLOCK.createInstance({ x, y, z }));
					}
				}
			}
		}

		//TODO: make this more secure (instead of invalidating pointer)
		RenderGroup& updateRender() noexcept {
			if (m_Render) delete m_Render;
			for (std::size_t x(0); x < UL_CHUNK_SIZE; ++x) {
				for (std::size_t y(0); y < UL_CHUNK_SIZE; ++y) {
					for (std::size_t z(0); z < UL_MAX_HEIGHT; ++z) {
						m_Models.push_back((*this)(x, y, z).getModel());
					}
				}
			}

			m_Render = new RenderGroup(Block::s_BlockMesh, UL_CHUNK_SIZE * UL_CHUNK_SIZE * UL_MAX_HEIGHT, m_Models.data());
			return *m_Render;
		}

		RenderGroup& getRender() noexcept {
			return *m_Render;
		}

		~Chunk() noexcept {
			if(m_Render)
				delete m_Render;
		}

	private:

		RenderGroup* m_Render;
		
		std::vector<glm::mat4> m_Models;
		std::vector<std::vector<std::vector<std::shared_ptr<BlockInstance>>>> m_Blocks;
		const Position m_Position;
		const id_t m_Id;

		inline static id_t s_CurrentId = 0;
		
	};*/
}