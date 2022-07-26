#pragma once
#include "game/GameObject.h"
#include "renderer/HlMesh.h"

namespace ul {
	class WorldObject;
	class WorldObject : public GameObject {

	public:
		template<typename _StrTy, typename _StrTy2,
			typename = std::enable_if_t<std::is_constructible_v<ulString, _StrTy>>,
			typename = std::enable_if_t<std::is_constructible_v<ulString, _StrTy2>>>
		WorldObject(_StrTy2&& domain, _StrTy&& name, const HlMesh& mesh, bool isOpaque=true) noexcept :
			GameObject(std::forward<_StrTy2>(domain), std::forward<_StrTy>(name)), m_Mesh(mesh), m_IsOpaque(isOpaque) {
			WORegistry.push_back(this);
		}

		virtual void initialize() noexcept override;

		inline const HlMesh& getMesh() const noexcept {
			return m_Mesh;
		}

		virtual HlMesh& getMesh() noexcept {
			return m_Mesh;
		}

		virtual bool isOpaque() const noexcept {
			return m_IsOpaque;
		}

		virtual WorldObject& setOpaque(bool opaque) noexcept {
			m_IsOpaque = opaque;
			return *this;
		}

		static std::vector<WorldObject*>& getWORegistry() noexcept {
			return WORegistry;
		}

	protected:
		bool m_IsOpaque;
	private:
		static std::vector<WorldObject*> WORegistry;
		HlMesh m_Mesh;
	};
}