#include "PhysicManager.h"

namespace ul {
	PhysicManager::PhysicManager(ErrorManager* err) : m_ErrorManager{ err } {
		linf << "Starting PhysX...\n";
		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_AllocatorCallback, m_ErrorCallback);
		if (!m_Foundation) {
			m_ErrorManager->copyError(Error{ ErrorLevel::FATAL, UL_S("PxCreateFoundation failed !!!\n") });
			UL_UNREACHABLE();
		}
		m_Pvd = PxCreatePvd(*m_Foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(UL_PVD_HOST, 5425, 10);
		m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale{}, gPhysXTracAllocs, m_Pvd);
		if (!m_Physics) {
			m_ErrorManager->copyError(Error{ ErrorLevel::FATAL, UL_S("PxCreatePhysics failed !!!\n") });
			UL_UNREACHABLE();
		}
		m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams{ physx::PxTolerancesScale{} });
		if (!m_Cooking) {
			m_ErrorManager->copyError(Error{ ErrorLevel::FATAL, UL_S("PxCreateCooking failed !!!\n") });
			UL_UNREACHABLE();
		}
		if (!PxInitExtensions(*m_Physics, m_Pvd)) {
			m_ErrorManager->copyError(Error{ ErrorLevel::FATAL, UL_S("PxInitExtensions failed !!!\n") });
			UL_UNREACHABLE();
		}

		m_CpuDispatcher = physx::PxDefaultCpuDispatcherCreate(UL_PHYSX_THREADS_NB);

		physx::PxCudaContextManagerDesc cudaContextManagerDesc;
		cudaContextManagerDesc.interopMode = physx::PxCudaInteropMode::OGL_INTEROP;

		m_CudaContextManager = PxCreateCudaContextManager(*m_Foundation, cudaContextManagerDesc, PxGetProfilerCallback());
		if (m_CudaContextManager) {
			if (!m_CudaContextManager->contextIsValid()) {
				UL_PHYSX_RELEASE(m_CudaContextManager);
			}
		}

		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3{ 0.f, -9.81f, 0.f };
		sceneDesc.cpuDispatcher = m_CpuDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		sceneDesc.cudaContextManager = m_CudaContextManager;
		//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS; //This should be activable in options
		//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM; //What about this ?
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
		//sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU; //This should be activable in options
		//sceneDesc.gpuMaxNumPartitions = 8; //Only for GPU

		m_Scene = m_Physics->createScene(sceneDesc);

		physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
		if (pvdClient) {
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, false);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, false);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, false);
		}

		//Will be moved in custom shape definition
		m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);

		//TEST PART
		physx::PxShape* shape = m_Physics->createShape(physx::PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *m_Material);
		for (physx::PxU32 x(0u); x < DEBUG_RENDER_X; ++x) {
			for (physx::PxU32 z(0u); z < DEBUG_RENDER_Y; ++z) {
				physx::PxTransform tm{ (float)x,0,(float)z };
				physx::PxRigidStatic* body = m_Physics->createRigidStatic(tm);
				m_Scene->addActor(*body);

			}
		}
		shape->release();
		linf << "PhysX initialization finished !\n";
	}

	physx::PxRigidDynamic* ul::PhysicManager::getCameraBody(physx::PxVec3 pos) {
		auto body = physx::PxCreateDynamic(*m_Physics, physx::PxTransform{ pos }, physx::PxBoxGeometry{ 0.5f, 0.5f, 1.f }, *m_Material, 80.f);
		m_Scene->addActor(*body);
		return body;
	}

	PhysicManager::~PhysicManager() {
		PxCloseExtensions();
		UL_PHYSX_RELEASE(m_Material);
		UL_PHYSX_RELEASE(m_Scene);
		UL_PHYSX_RELEASE(m_CpuDispatcher);
		UL_PHYSX_RELEASE(m_CudaContextManager);
		UL_PHYSX_RELEASE(m_Cooking);
		UL_PHYSX_RELEASE(m_Physics);
		if (m_Pvd) {
			physx::PxPvdTransport* transport = m_Pvd->getTransport();
			UL_PHYSX_RELEASE(m_Pvd);
			UL_PHYSX_RELEASE(transport);
		}
		UL_PHYSX_RELEASE(m_Foundation);
	}

	UlPxAllocator PhysicManager::m_AllocatorCallback;
	UlPxErrorCallback PhysicManager::m_ErrorCallback;
}