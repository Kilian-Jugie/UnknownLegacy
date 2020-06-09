#pragma once
#include <vector>
#include <future>
#include "filesystem/AssetsManager.h"
#include "utils/ErrorManager.h"
#include "utils/Logger.h"
//#include "physic/PhysicManager.h"

namespace ul {
	enum class ReturnCodes : int {
		NOERROR
	};

	class UnknownLegacy {
	public:
		using log_char_t = ulchar_t;

		static UnknownLegacy& getInstance() {
			static UnknownLegacy INSTANCE;
			return INSTANCE;
		}

		UnknownLegacy(const UnknownLegacy&) = delete;
		UnknownLegacy(UnknownLegacy&&) = delete;

		~UnknownLegacy() = default;
		
		UnknownLegacy& operator=(const UnknownLegacy&) = delete;
		UnknownLegacy& operator=(UnknownLegacy&&) = delete;

		void setupLocations();
		void loadBasicAssets();

		ReturnCodes start();
		void terminate();

		bool processError(Error& e);
		bool processInfo(Error& e);
		bool processFatal(Error& e);
		bool processWarning(Error& e);
		bool processDebug(Error& e);

		AssetsManager& getAssetsManager() noexcept {
			return m_AssetsManager;
		}

	private:
		UnknownLegacy() : m_ErrorManager{}, m_AssetsManager{ UL_S("assets"), m_ErrorManager }, m_ShaderLocationId{ 0 }/*, m_PhysicManager{ nullptr }*/ {
		};
		
		AssetsManager m_AssetsManager;
		ErrorManager m_ErrorManager;
		size_t m_ShaderLocationId;
		//PhysicManager* m_PhysicManager;
		
	};
}