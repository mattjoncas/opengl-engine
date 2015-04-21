#include "Scene.h"

namespace mor{

	Scene::Scene(){
		isRunning = true;
		//Load();
	}

	Scene::~Scene(){

	}
	
	void Scene::SetWindowSize(int sWidth, int sHeight){
		renderer.SetScreenSize(sWidth, sHeight);
	}
	void Scene::SwitchScene(std::string next_scene){
		return_message = next_scene;
		isRunning = false;
	}

	void Scene::Load(){
		std::cout << "base load.\n";
	}
	void Scene::Update(float _delta){
		renderer.Update(_delta);
		Input();
	}
	void Scene::Render(){
		std::cout << "base render.\n";
	}

	void Scene::Input(){
		// esc closes window
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
			isRunning = false;
		}
	}

}