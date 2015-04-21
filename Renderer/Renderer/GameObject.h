#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "BoundingShape.h"
#include "Sphere.h"

namespace mor{

	class GameObject {
	public:
		GameObject();
		GameObject(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale, glm::vec3 _vel, glm::vec3 _accel, glm::vec3 _angVel);
		~GameObject();
		void Init(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale);
		void Init(int _model, int _shader, int _texture, int _material);

		glm::vec3 position;
		glm::quat q;
		glm::vec3 rotation;
		glm::vec3 scale;

		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 angularVel;

		virtual void Update(float _delta);
		
		inline bool IsActive(){
			return active;
		}
		inline void SetActive(bool _isActive){
			active = _isActive;
		}
		inline void SetRotation(glm::quat _q){
			q = _q;
		}
		inline void SetRotation(glm::vec3 _rot){
			q = glm::quat(_rot);
		}

		int texture, shader, model, material, normal_map;

		glm::mat4x4 GetModelMatrix();

		void AddChild(GameObject* _child);
		std::vector<GameObject*> GetChildren();
		//get child object at index
		GameObject* GetChild(int _index);

		BoundingShape *bounding_shape;
		
	private:
		GameObject* parent;
		std::vector<GameObject*> children;

		bool active;

		void CreateBoundingSphere();
	};

}