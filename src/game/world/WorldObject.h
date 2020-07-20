#pragma once
#include "../GameObject.h"
#include "../renderer/HlMesh.h"

namespace ul {
	class WorldObject : public GameObject {

	public:
		template<typename _StrTy, typename = std::enable_if_t<std::is_constructible_v<ulString, _StrTy>>>
		WorldObject(_StrTy&& name, const HlMesh& mesh, bool isOpaque=true) noexcept : GameObject(std::forward<_StrTy>(name)), m_Mesh(mesh), m_IsOpaque(isOpaque) {}
		
		/*
			onLoaded
			onUnLoaded
		*/

		inline const HlMesh& getMesh() const noexcept {
			return m_Mesh;
		}

		inline bool isOpaque() const noexcept {
			return m_IsOpaque;
		}

	private:
		const HlMesh& m_Mesh;
		bool m_IsOpaque;
	};
}