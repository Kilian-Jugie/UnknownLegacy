#pragma once
#include "../GameObject.h"
#include "../renderer/Mesh.h"

namespace ul {
	class WorldObject : public GameObject {

	protected:
		template<typename _StrTy, typename = std::enable_if_t<std::is_constructible_v<ulString, _StrTy>>>
		WorldObject(_StrTy&& name, const Mesh& mesh) noexcept : GameObject(std::forward<_StrTy>(name)), m_Mesh(mesh) {}
		
		/*
			onLoaded
			onUnLoaded
		*/

		const Mesh& getMesh() const {
			return m_Mesh;
		}

		const Mesh& m_Mesh;
	};
}