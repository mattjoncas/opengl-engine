#pragma once
#include "Scene.h"

#include <vector>

namespace mor{
	class SceneManager{
	private:
		static SceneManager *instance;
		SceneManager();

		bool isRunning;

		std::vector<Scene*> scenes;
		Scene* current_scene = nullptr;
	public:
		~SceneManager();
		static SceneManager& GetInstance();
		static void ResetInstance();

		/*
		func: add a scene to the scene manager.
		*/
		void AddScene(Scene *_newScene);
		/*
		func: bind a scene to the scene manager.
		return: true if scene was binded.
		*/
		bool BindScene(Scene *_scene);
		/*
		func: get the currently binded scene from SceneManager
		call your override or base Update/Render methods from this
		returns: the current scene
		*/
		Scene* C_Scene();

		inline bool IsRunning(){
			return isRunning;
		}

		void Update();
	};

}