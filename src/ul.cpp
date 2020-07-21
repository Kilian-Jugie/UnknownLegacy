#include "ul.h"
//#include "filesystem/AssetLocation.h"
#include "utils/Logger.h"

#include <iostream>

#include "utils/debug.h"
#include "renderer/Renderer.h"
#include "game/world/WorldObject.h"


int main() {
	

	ul::UnknownLegacy& game = ul::UnknownLegacy::getInstance();
	int r = static_cast<int>(game.start());
	//system("pause");
	return r;
}

bool errorCallback(ul::Error& e) {
	return ul::UnknownLegacy::getInstance().processError(e);
}

bool infoCallback(ul::Error& e) {
	return ul::UnknownLegacy::getInstance().processInfo(e);
}

bool fatalCallback(ul::Error& e) {
	return ul::UnknownLegacy::getInstance().processFatal(e);
}

bool warningCallback(ul::Error& e) {
	return ul::UnknownLegacy::getInstance().processWarning(e);
}

bool debugCallback(ul::Error& e) {
	return ul::UnknownLegacy::getInstance().processDebug(e);
}


void ul::UnknownLegacy::setupLocations() {
	//AssetLocation shaderLoc("shaders", m_AssetsManager.getAssetsPath());
	m_ShaderLocationId = m_AssetsManager.addFolder("shaders");
	//linf << "Shaders location set to: " << m_AssetsManager.getLocation(m_ShaderLocationId).getPath().string() << "\n";
}

void ul::UnknownLegacy::loadBasicAssets() {

}

ul::ReturnCodes ul::UnknownLegacy::start() {
	linf << "Starting UnknownLegacy yeay !\n";

	m_ErrorManager.addCallback(ErrorLevel::ERROR, errorCallback);
	m_ErrorManager.addCallback(ErrorLevel::INFO, infoCallback);
	m_ErrorManager.addCallback(ErrorLevel::FATAL, fatalCallback);
	m_ErrorManager.addCallback(ErrorLevel::WARNING, warningCallback);
	m_ErrorManager.addCallback(ErrorLevel::DEBUG, debugCallback);

	std::atexit([] {linf << "Exiting...\n"; });

	this->setupLocations();

	auto texsLocationId = m_AssetsManager.addFolder("textures");
	Renderer::getInstance().getTextureManager().addFolder("ul", m_AssetsManager.getPath(texsLocationId));
	
	auto fRenderer = std::async(std::launch::async, &Renderer::render, &Renderer::getInstance(),
		std::ref(m_AssetsManager), m_ShaderLocationId); 
	linf << "Render started !\n";
	int code = fRenderer.get();
	linf << "Finished Render with code: " << code << "\n";

	//m_PhysicManager = nullptr;
	
	return ReturnCodes::NOERROR;
}

void ul::UnknownLegacy::terminate() {
	std::exit(EXIT_SUCCESS);
}

namespace ul {
	bool UnknownLegacy::processError(Error& e) {
		lerr << e.getDescription();
		return true;
	}

	bool UnknownLegacy::processInfo(Error& e) {
		linf << e.getDescription();
		return true;
	}

	bool UnknownLegacy::processFatal(Error& e) {
		lfat << e.getDescription();
		lfat << "A fatal error has been throwed, process must stop !\n";
		terminate();
		UL_UNREACHABLE();
		return false;
	}

	bool UnknownLegacy::processWarning(Error& e) {
		lwar << e.getDescription();
		return true;
	}

	bool UnknownLegacy::processDebug(Error& e) {
		ldeb << e.getDescription();
		return true;
	}
}
