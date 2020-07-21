#include "WorldObject.h"
#include "../renderer/Renderer.h"

void ul::WorldObject::initialize() noexcept {
	std::size_t texId = Renderer::getInstance().getTextureManager().getTextureId(getDomain(), getName());
	m_Mesh.apply([texId](HlMesh::face_t& face) { face.texId = texId; });
}
