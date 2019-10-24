#pragma once
#include <foundation/PxErrorCallback.h>
#include <foundation/PxErrors.h>
#include "../utils/Logger.h"

namespace ul {
	class UlPxErrorCallback : public physx::PxErrorCallback {
	public:
		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) noexcept override final  {
			lerr << "[PhysX]" << file << "(" << line << "): error " << static_cast<int>(code) << ": " << message << "\n";
		}
	};
}