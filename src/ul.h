#pragma once
#include "filesystem/AssetsManager.h"
#include "utils/ErrorManager.h"
#include "utils/Logger.h"
#include <vector>
#include <future>

//#include "physic/PhysicManager.h"

namespace ul {
	class WorldObject;

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

		void registerWorldObject(WorldObject* go) noexcept;

		void initializeWorldObjects() noexcept;

	private:
		UnknownLegacy() : m_ErrorManager{}, m_AssetsManager{ std::filesystem::current_path() / "Data" }, m_ShaderLocationId{ 0 }/*, m_PhysicManager{ nullptr }*/ {
		};
		
		AssetsManager m_AssetsManager;
		ErrorManager m_ErrorManager;
		AssetsManager::id_t m_ShaderLocationId;
		std::vector<WorldObject*> m_WorldObjectsRegistry;
		
		//PhysicManager* m_PhysicManager;
		
	};
}