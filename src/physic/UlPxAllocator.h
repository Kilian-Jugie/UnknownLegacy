#pragma once
#include <malloc.h>
#include "../global.h"
#include "foundation/PxAllocatorCallback.h"

#define _UL_PHYSX_ALIGN_RULE 16

namespace ul {
	class UlPxAllocator : public physx::PxAllocatorCallback {
		static void* platformAlignedAlloc(size_t size) {
			return _aligned_malloc(size, _UL_PHYSX_ALIGN_RULE);
		}

		static void platformAlignedFree(void* ptr) {
			_aligned_free(ptr);
		}

	public:
		void* allocate(size_t size, const char*, const char*, int) {
			void* ptr = platformAlignedAlloc(size);
			UL_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0, "Malloc alignement errored !");
			return ptr;
		}

		void deallocate(void* ptr)
		{
			platformAlignedFree(ptr);
		}
	};
}