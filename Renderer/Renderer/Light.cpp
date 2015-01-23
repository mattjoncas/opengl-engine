#include "Light.h"

namespace mor{
	Light::Light(){

	}
	Light::Light(glm::vec4 _position, glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular, float _size, float _drop_off){
		position = _position;
		ambient = _ambient;
		diffuse = _diffuse;
		specular = _specular;

		size = _size;
		drop_off_rate = _drop_off;

		isOn = true;
		update_ubo = true;
	}

	Light::~Light(){

	}

	void Light::SetPosition(glm::vec4 _pos){
		position = _pos;

		update_ubo = true;
	}
	void Light::SetAmbient(glm::vec4 _amb){
		ambient = _amb;

		update_ubo = true;
	}
	void Light::SetDiffuse(glm::vec4 _diff){
		diffuse = _diff;

		update_ubo = true;
	}
	void Light::SetSpecular(glm::vec4 _spec){
		specular = _spec;

		update_ubo = true;
	}

	bool Light::UpdateCheck(){
		if (update_ubo){
			update_ubo = false;
			return true;
		}
		return false;
	}
}