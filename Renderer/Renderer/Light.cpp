#include "Light.h"

namespace mor{
	Light::Light(){

	}
	Light::Light(glm::vec3 _position, glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular, float _size, float _drop_off, bool cast_shadows){
		if (!cast_shadows){ position = glm::vec4(_position, 0.0f); }
		else{ position = glm::vec4(_position, 1.0f); }
		direction = glm::vec4(0.0f);
		ambient = _ambient;
		diffuse = _diffuse;
		specular = _specular;

		size = _size;
		drop_off_rate = _drop_off;
		isOn = true;
		update_ubo = true;
	}
	Light::Light(glm::vec3 _direction, glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular){
		position = glm::vec4(0.0f);
		direction = glm::vec4(_direction, 0.0f);
		ambient = _ambient;
		diffuse = _diffuse;
		specular = _specular;

		size = 0.0f;
		drop_off_rate = 0.0f;
		isOn = true;
		update_ubo = true;
	}

	Light::~Light(){

	}

	void Light::SetPosition(glm::vec4 _pos){
		position = _pos;

		update_ubo = true;
	}
	void Light::SetDirection(glm::vec4 _dir){
		position = _dir;

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