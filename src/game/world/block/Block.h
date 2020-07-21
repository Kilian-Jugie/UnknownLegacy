#pragma once
#include "../../fwd.h"
#include "../WorldObject.h"
#include "../renderer/StaticVertices.h"

namespace ul {
	
	class Block : public WorldObject {
	public:
		template<typename _StrTy, typename _StrTy2,
			typename = std::enable_if_t<std::is_constructible_v<ulString, _StrTy>>,
			typename = std::enable_if_t<std::is_constructible_v<ulString, _StrTy2>>>
		Block(_StrTy2&& domain, _StrTy&& name) noexcept : WorldObject(
			std::forward<_StrTy2>(domain), std::forward<_StrTy>(name),
				gBlockHlMesh), m_BlockId{ ++s_CurrentId } {}

		inline id_t getBlockId() const noexcept {
			return m_BlockId;
		}

		inline Block& setOpaque(bool opaque) {
			m_IsOpaque = opaque;
			return *this;
		}

		private:
			inline static id_t s_CurrentId = 0;
			const id_t m_BlockId;
	};

	namespace Blocks {
		Block STONE{ UL_STR("ul"), UL_STR("stone") };
		Block AIR{ UL_STR("ul"), UL_STR("air") };
	}
}