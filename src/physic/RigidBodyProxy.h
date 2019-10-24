#pragma once
#include <PxPhysicsAPI.h>

namespace ul {
	template<typename _BodyT>class _RigidProxy {
	public:
		_RigidProxy(_BodyT* b) : m_Body{b} {}
	protected:
		_BodyT* m_Body;
	};


	class RigidDynamicProxy : public physx::PxRigidDynamic {
	public:
		RigidDynamicProxy(const physx::PxRigidDynamic& dyn) : physx::PxRigidDynamic{dyn} { }

		

	private:

	};
}