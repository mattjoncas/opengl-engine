#pragma once
#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Light.h"

#define MAX_LIGHTS 10

namespace mor{
	class LightManager{
	private:
		static LightManager *instance;

		LightManager();

		std::vector<Light*> lights;
	public:
		~LightManager();

		static LightManager& GetInstance();

		void AddLight(Light* _newLight);
		void RemoveLight(Light* _light);
		void UpdateUniforms(GLuint _light_ubo);
		int LightCount();

		glm::vec3 GetLightPosition(int _index);
	};

}