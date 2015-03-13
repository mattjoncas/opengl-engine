#pragma once

#include <glm/glm.hpp>

namespace mor{
	class Light
	{
	public:
		Light();
		//point light
		Light(glm::vec3 _position, glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular, float _size, float _drop_rate, bool cast_shadows);
		//directional light
		Light(glm::vec3 _direction, glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular);
		~Light();

		inline glm::vec4 Position() { return position; };
		inline glm::vec4 Direction() { return direction; };
		inline glm::vec4 Ambient() { return ambient;  };
		inline glm::vec4 Diffuse() { return diffuse; };
		inline glm::vec4 Specular() { return specular; };

		void SetPosition(glm::vec4 _pos);
		void SetDirection(glm::vec4 _dir);
		void SetAmbient(glm::vec4 _amb);
		void SetDiffuse(glm::vec4 _diff);
		void SetSpecular(glm::vec4 _spec);

		inline bool ToggleOn(){
			if (isOn){
				isOn = false;
			}
			else{
				isOn = true;
			}
			update_ubo = true;

			return isOn;
		}
		inline void SetOn(bool _isOn){
			isOn = _isOn;
		}
		inline bool IsOn(){
			return isOn;
		}
		bool UpdateCheck();

		//public for now
		float size, drop_off_rate;
	private:

		glm::vec4 position, direction, ambient, diffuse, specular;

		bool isOn, update_ubo;
	};

}