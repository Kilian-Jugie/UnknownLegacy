#pragma once
#include <glm/vec3.hpp>
#include <foundation/PxVec3.h>
#include <foundation/PxTransform.h>
#include <type_traits>
#include "../global.h"

/*
	To remove when https://github.com/g-truc/glm/issues/594 fixed
*/
#ifdef GLM_CONSTEXPR_CTOR
#define UL_VEC3_CEXPR_CTOR GLM_CONSTEXPR_CTOR
#else
#define UL_VEC3_CEXPR_CTOR
#endif

namespace ul {
	class vec3 {
	public:
		vec3(float x, float y, float z) noexcept : m_V{ x,y,z } {}

		template<typename _glm_vec3_t,
			typename = std::enable_if_t<std::is_same_v<std::decay_t<_glm_vec3_t>, glm::vec3>>>
			UL_VEC3_CEXPR_CTOR vec3(_glm_vec3_t&& v) noexcept : m_V{ std::forward<glm::vec3>(v) } {}

		vec3(const physx::PxVec3& v) noexcept : m_V{v.x, v.y, v.z} {}

		vec3(const physx::PxTransform& t) noexcept : vec3(t.p) { }

		template<typename _physx_trans_t,
			typename = std::enable_if_t<std::is_same_v<std::decay_t<_physx_trans_t>, glm::vec3>>>
			vec3(_physx_trans_t&& v) noexcept : vec3(std::forward<physx::PxVec3>(v.p)) {}


		UL_FORCEINLINE glm::vec3 getGlmVec3() {
			return m_V;
		}

		UL_FORCEINLINE physx::PxVec3 getPhysxVec3() {
			return physx::PxVec3{ m_V.x, m_V.y, m_V.z };
		}

		UL_FORCEINLINE physx::PxTransform getPhysxTransform() {
			return physx::PxTransform{ getPhysxVec3() };
		}

		UL_FORCEINLINE operator glm::vec3() {
			return getGlmVec3();
		}

		UL_FORCEINLINE operator physx::PxVec3() {
			return getPhysxVec3();
		}

		UL_FORCEINLINE operator physx::PxTransform() {
			return getPhysxTransform();
		}

		UL_FORCEINLINE const float& x() const noexcept {
			return m_V.x;
		}

		UL_FORCEINLINE const float& y() const noexcept {
			return m_V.y;
		}

		UL_FORCEINLINE const float& z() const noexcept {
			return m_V.z;
		}

	private:
		glm::vec3 m_V;
	};

#ifndef UL_NO_VEC3_PROXY_BIND
#include "../utils/ProxyBind.h"

	template<typename _Ty2>class ul::ProxyBind<glm::vec3, _Ty2> {

	};

#endif // !UL_NO_VEC3_PROXY_BIND


	
}