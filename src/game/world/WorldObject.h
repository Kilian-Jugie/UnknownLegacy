#pragma once
#include "../GameObject.h"
#include "../renderer/HlMesh.h"

namespace ul {
	class WorldObject : public GameObject {

	public:
		template<typename _StrTy, typename = std::enable_if_t<std::is_constructible_v<ulString, _StrTy>>>
		WorldObject(_StrTy&& name, const HlMesh& mesh) noexcept : GameObject(std::forward<_StrTy>(name)), m_Mesh(mesh) {}
		
		/*
			onLoaded
			onUnLoaded
		*/

		const HlMesh& getMesh() const {
			return m_Mesh;
		}

		const HlMesh& m_Mesh;
	};
}