#pragma once
#include <Types.h>
#include <game/Position.h>
#include "block/Block.h"
#include "block/BlockInstance.h"
#include "Constants.h"
#include <vector>
#include <functional>
#include "../renderer/RenderGroup.h"
#include "../renderer/StaticVertices.h"


namespace ul {
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