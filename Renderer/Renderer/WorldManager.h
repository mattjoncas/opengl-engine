#pragma once
#include "Renderer.h"
#include "GameObject.h"
#include <iostream>

namespace mor{

	class WorldManager{
	protected:
		bool isRunning;
		Renderer renderer;
		std::vector<GameObject*> objects;

		virtual void Input();
	private:
		//static WorldManager *instance;

		//WorldManager();

		virtual void Load();
	public:
		WorldManager();
		~WorldManager();

		//static WorldManager& GetInstance();
		bool IsRunning();

		virtual void Render();
		virtual void Update(float _delta);
	};

}