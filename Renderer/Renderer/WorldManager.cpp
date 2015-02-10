#include "WorldManager.h"

namespace mor{

	//WorldManager* WorldManager::instance = nullptr;

	WorldManager::WorldManager(){
		isRunning = true;
		//Load();
	}

	WorldManager::~WorldManager(){

	}
	/*
	WorldManager& WorldManager::GetInstance(){
		if (instance == nullptr){
			instance = new WorldManager();
		}
		return *instance;
	}*/
	void WorldManager::SetWindowSize(int sWidth, int sHeight){
		renderer.SetScreenSize(sWidth, sHeight);
	}
	bool WorldManager::IsRunning(){
		return isRunning;
	}

	void WorldManager::Load(){
		std::cout << "base load.\n";
	}
	void WorldManager::Update(float _delta){
		renderer.Update(_delta);
		Input();
	}
	void WorldManager::Render(){
		std::cout << "base render.\n";
	}

	void WorldManager::Input(){
		// esc closes window
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
			isRunning = false;
		}
	}

}