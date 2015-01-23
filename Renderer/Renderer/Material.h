#pragma once

#include <glm/glm.hpp>

namespace mor{
	class Material{
	public:
		//Material();
		Material(glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular, float _shininess);
		~Material();

		glm::vec4 material_ambient;
		glm::vec4 material_diffuse;
		glm::vec4 material_specular;
		float material_shininess;
	};

}