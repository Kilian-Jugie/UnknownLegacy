#pragma once
#include "../../fwd.h"
#include "../InstancedObject.h"
#include "Block.h"

namespace ul {
	class BlockInstance : public InstancedObject {
	public:
		BlockInstance(Block& from, Position& pos) noexcept : InstancedObject{ pos }, m_Block { from } {}

		const Block const& getBlock() const noexcept {
			return m_Block;
		}

	private:
		Block const& m_Block;
	};
}