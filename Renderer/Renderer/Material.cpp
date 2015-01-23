#include "Material.h"

namespace mor{
	/*Material::Material(){

	}*/
	Material::Material(glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular, float _shininess){
		material_ambient = _ambient;
		material_diffuse = _diffuse;
		material_specular = _specular;
		material_shininess = _shininess;
	}

	Material::~Material(){

	}

}