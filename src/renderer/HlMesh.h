#pragma once
#include <vector>
#include "Mesh.h"

namespace ul {
	class HlMesh {
	public:
		struct vertex {
			float pos[3];
			float tex[2];
		};

		HlMesh(std::vector<std::vector<vertex>> faces) : m_Faces{ std::move(faces) }, eqVertices{  } {}

		~HlMesh() {
			//if (eqVertices) delete[] eqVertices;
		}

		enum class Faces : unsigned {
			NORTH,
			EAST,
			SOUTH,
			WEST,
			UP,
			DOWN
		};

		std::vector<vertex> get(Faces face) const {
			return m_Faces[(unsigned)face];
		}

		static std::pair<std::vector<float>, std::vector<float>> vertexArrayAsDualArray(const std::vector<vertex>& ver) {
			std::pair<std::vector<float>, std::vector<float>> ret;
			for (auto it : ver) {
				ret.first.insert(ret.first.end(), std::begin(it.pos), std::end(it.pos));
				ret.second.insert(ret.second.end(), std::begin(it.tex), std::end(it.tex));
			}
			return ret;
		}

		Mesh getAsMesh(Faces face) const {
			auto d{ vertexArrayAsDualArray(get(face)) };
			return Mesh(d.first.data(), d.first.size() * sizeof(float), d.second.data(), d.second.size(), nullptr, 0);
		}

		//DEBUG TO REMOVE
		Mesh toMesh() {
			unsigned index = 0;
			for (auto it: m_Faces) {
				for (auto fit: it) {
					eqVertices.first.insert(eqVertices.first.end(), std::begin(fit.pos), std::end(fit.pos));
					eqVertices.second.insert(eqVertices.second.end(), std::begin(fit.tex), std::end(fit.tex));
				}
			}

			//GLushort indices[] = {};
			return std::move(Mesh(eqVertices.first.data(), eqVertices.first.size()*sizeof(float), eqVertices.second.data(), eqVertices.second.size()*sizeof(float), nullptr, 0));
		}

	private:
		std::pair<std::vector<float>, std::vector<float>> eqVertices;

		std::vector<std::vector<vertex>> m_Faces;
	};
}