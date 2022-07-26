#include "WorldObject.h"
#include "renderer/Renderer.h"

void ul::WorldObject::initialize() noexcept {
	const HlMeshFace::texid_t texId = static_cast<HlMeshFace::texid_t>(
		Renderer::getInstance().getTextureManager().getTextureId(getDomain(), getName()));
	m_Mesh.apply([texId](HlMesh::face_t& face) { face.texId = texId; });
}

namespace ul {
	std::vector< WorldObject*> WorldObject::WORegistry = {};
}
