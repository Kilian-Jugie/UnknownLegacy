#pragma once
#include "utils/Types.h"
#include "Chunk.h"
#include <vector>

namespace ul {
	class World {
	public:

	private:
		std::vector<std::vector<Chunk>> m_Chunks;
		ulString m_Name;
		id_t m_Id;
	};
}