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
		Chunk(const Position& pos) : m_Position{ pos }, block{"block", gBlockHlMesh} {
			initBlocks();
		}

		~Chunk() {
			deleteBlocks();
		}

		void initBlocks() {
			for (int x(0); x < 16; ++x) {
				for (int y(0); y < 16; ++y) {
					for (int z(0); z < 16; ++z) {
						blocks[x][y][z] = new InstancedObject(glm::mat4(), block);
						glm::translate(blocks[x][y][z]->model, { x,y,z });
						glm::scale(blocks[x][y][z]->model, { 0.5f, 0.5f, 0.5f });
					}
				}
			}
		}

		void deleteBlocks() {
			for (int x(0); x < 16; ++x) {
				for (int y(0); y < 16; ++y) {
					for (int z(0); z < 16; ++z) {
						delete blocks[x][y][z];
					}
				}
			}
		}

		std::vector<glm::mat4> getModels() {
			std::vector<glm::mat4> ret;
			for (int x(0); x < 16; ++x) {
				for (int y(0); y < 16; ++y) {
					for (int z(0); z < 16; ++z) {
						ret.push_back(blocks[x][y][z]->model);
					}
				}
			}
			return ret;
		}

		std::vector<HlMesh::vertex>& translate(std::vector<HlMesh::vertex>& vec, float translation[3]) {
			for (auto& it : vec) {
				for (size_t i(0); i < 3; ++i) {
					it.pos[i] += translation[i];
				}
			}
			return vec;
		}

		Mesh toMesh(HlMesh* blockMesh) {
			
			std::vector<HlMesh::vertex> vertices;
			std::vector<HlMesh::vertex> tmpV;
			for (int x(0); x < 16; ++x) {
				for (int y(0); y < 16; ++y) {
					for (int z(0); z < 16; ++z) {
						tmpV.clear();
						if (x == 0) {
							auto v = block.getMesh().get(HlMesh::Faces::WEST);
							tmpV.insert(tmpV.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
						}
						else if (x == 15) {
							 auto v = block.getMesh().get(HlMesh::Faces::EAST);
							 tmpV.insert(tmpV.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
							//vertices.insert(vertices.end(), tmpV.begin(), tmpV.end());
						}
						if (y == 0) {
							 auto v = block.getMesh().get(HlMesh::Faces::UP);
							 tmpV.insert(tmpV.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
							//vertices.insert(vertices.end(), tmpV.begin(), tmpV.end());
						}
						else if (y == 15) {
							 auto v = block.getMesh().get(HlMesh::Faces::DOWN);
							 tmpV.insert(tmpV.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
							//vertices.insert(vertices.end(), tmpV.begin(), tmpV.end());
						}
						if (z == 0) {
							 auto v = block.getMesh().get(HlMesh::Faces::SOUTH);
							 tmpV.insert(tmpV.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
							//vertices.insert(vertices.end(), tmpV.begin(), tmpV.end());
						}
						else if (z == 15) {
							 auto v = block.getMesh().get(HlMesh::Faces::NORTH);
							 tmpV.insert(tmpV.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
							//vertices.insert(vertices.end(), tmpV.begin(), tmpV.end());
						}
						if (!tmpV.empty()) {
							float tr[] = { x,y,z };
							tmpV = translate(tmpV, tr);
							vertices.insert(vertices.end(), tmpV.begin(), tmpV.end());
						}
					}
				}
			}

			auto vert = HlMesh::vertexArrayAsDualArray(vertices);
			

			return Mesh(vert.first.data(), vert.first.size() * sizeof(float), vert.second.data(), vert.second.size()*sizeof(float), nullptr, 0);
		}

	private:
		const Position m_Position;
		InstancedObject* blocks[16][16][16];
		WorldObject block;
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