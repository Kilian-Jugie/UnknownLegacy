#pragma once
#include "../../fwd.h"
#include "../WorldObject.h"
#include "../renderer/StaticVertices.h"
//#include "BlockInstance.h"

namespace ul {
	
	/*class Block : public WorldObject {
	public:
		template<typename _StrTy, typename = std::enable_if_t<std::is_constructible_v<ulString, _StrTy>>>
		Block(_StrTy&& name) noexcept : WorldObject(std::forward<_StrTy>(name), 
			s_BlockMesh), m_BlockId{++s_CurrentId} {}

		id_t getBlockId() const noexcept {
			return m_BlockId;
		}

		std::shared_ptr<BlockInstance> createInstance(Position p) {
			return std::make_shared<BlockInstance>(*this, std::move(p));
		}

		inline static Mesh s_BlockMesh{ gCubeVertices, sizeof(gCubeVertices), gCubeIndices, sizeof(gCubeIndices) };

		private:
			inline static id_t s_CurrentId = 0;

		protected:
			const id_t m_BlockId;
	};

	namespace Blocks {
		Block TEST_BLOCK{ UL_STR("TEST_BLOCK") };
	}*/
}