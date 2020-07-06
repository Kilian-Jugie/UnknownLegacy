#include "Mesh.h"
#include "Renderer.h"

namespace ul {
	void Mesh::genDrawCommand() {
		Renderer::IndirectDrawCmd testDraw;
		testDraw.vertexCount = m_VerticesSize / 5; //Calculation to be confirmed
		testDraw.instanceCount = DEBUG_RENDER_AMOUNT;
		testDraw.firstIndex = 0;
		testDraw.baseInstance = 0;
		testDraw.baseVertex = 0;

		unsigned indirectBuffer;

		glGenBuffers(1, &indirectBuffer);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(testDraw), &testDraw, GL_STATIC_DRAW);
	}
}