#pragma once
#include <gl/glew.h>

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"
#include "Light.h"

namespace mor{
	class MaterialManager{
	private:
		static MaterialManager *instance;
		std::vector<Material> materials;

		MaterialManager();

		int current;
	public:
		~MaterialManager();

		static MaterialManager& GetInstance();
		int AddMaterial(glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular, float _shininess);

		void BindMaterial(int _index, GLuint _ubo);
	};

}