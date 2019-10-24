#pragma once
#include <PxPhysicsAPI.h>
#include "UlPxErrorCallback.h"
#include "UlPxAllocator.h"
#include "../utils/ErrorManager.h"
#include "../global.h"

namespace ul {
#ifdef UL_PHYSX_TRACK_ALLOCS
	static constexpr bool gPhysXTracAllocs = true;
#else
	static constexpr bool gPhysXTracAllocs = false;
#endif // UL_PHYSX_TRACK_ALLOCS

	class PhysicManager {
		PhysicManager(ErrorManager* err);
		~PhysicManager();

	public:

		void simulate(float elapsed) {
			m_Scene->simulate((physx::PxReal)elapsed);
			m_Scene->fetchResults(true);
		}

		static PhysicManager& get(ErrorManager* err = nullptr) {
			static PhysicManager instance{ 
				[=]() {UL_ASSERT(err,"An ErrorManager must be provided for the first call of get !!!"); return err; }() 
			};
			return instance;
		}

		physx::PxRigidDynamic* getCameraBody(physx::PxVec3 pos);


	private:
		ErrorManager* m_ErrorManager;
		physx::PxMaterial* m_Material;
		physx::PxScene* m_Scene;
		physx::PxFoundation* m_Foundation;
		physx::PxPvd* m_Pvd;
		physx::PxPhysics* m_Physics;
		physx::PxCooking* m_Cooking;
		physx::PxDefaultCpuDispatcher* m_CpuDispatcher;
		physx::PxCudaContextManager* m_CudaContextManager;
		static UlPxAllocator m_AllocatorCallback;
		static UlPxErrorCallback m_ErrorCallback;
	};

	

}