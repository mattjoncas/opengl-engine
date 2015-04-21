#pragma once
#include "Renderer.h"
#include "GameObject.h"
#include <iostream>

namespace mor{

	class Scene{
	protected:
		bool isRunning;
		std::string return_message;
		Renderer renderer = Renderer::GetInstance();
		std::vector<GameObject*> objects;

		/*
		func: used to switch scenes or stop all scenes from within a scene.
		requires use of SceneManager to work.
		**to exit program and stop SceneManager pass "exit" as next_scene.**
		param:
		the name of the scene you wish to swtich to, as it is referred to in SceneManager
		*/
		void SwitchScene(std::string next_scene);

		virtual void Input();
	private:
		virtual void Load();
	public:
		Scene();
		~Scene();
		//must set window size before rendering / loading
		void SetWindowSize(int sWidth, int sHeight);
		
		inline bool IsRunning(){
			return isRunning;
		}
		inline std::string ReturnMessage(){
			return return_message;
		}


		virtual void Render();
		virtual void Update(float _delta);
	};

}