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

		const HlMesh& getMesh() const {
			return m_Mesh;
		}

		bool isOpaque() const {
			return m_IsOpaque;
		}

		const HlMesh& m_Mesh;

	private:
		bool m_IsOpaque;
	};
}