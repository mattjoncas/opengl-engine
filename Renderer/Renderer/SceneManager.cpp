#include "SceneManager.h"

namespace mor{
	SceneManager* SceneManager::instance = nullptr;

	SceneManager::SceneManager(){
		isRunning = true;
	}

	SceneManager::~SceneManager(){
		for (std::vector<Scene*>::iterator iter = scenes.begin(); iter != scenes.end(); ++iter){
			delete((*iter));
		}
	}

	SceneManager& SceneManager::GetInstance(){
		if (instance == nullptr){
			instance = new SceneManager();
		}
		return *instance;
	}
	void SceneManager::ResetInstance(){
		delete instance;
		instance = nullptr;
	}

	void SceneManager::AddScene(Scene *_newScene){
		scenes.push_back(_newScene);
	}
	bool SceneManager::BindScene(Scene *_scene){
		for (int i = 0; i < int(scenes.size()); i++){
			if (_scene == scenes[i]){
				current_scene = scenes[i];
				return true;
			}
		}
		current_scene = nullptr;
		return false;
	}
	Scene* SceneManager::C_Scene(){
		return current_scene;
	}

	void SceneManager::Update(){
		if (!current_scene->IsRunning()){
			if (current_scene->ReturnMessage() == "exit"){
				isRunning = false;
			}
			else{
				//search scenes for name

			}
		}
	}
}