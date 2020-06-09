#pragma once
#include <Types.h>
#include <vector>
#include "Chunk.h"

namespace ul {
	class World {
	public:
		

	private:
		std::vector<std::vector<Chunk>> m_Chunks;
		ulString m_Name;
		id_t m_Id;
	};
}