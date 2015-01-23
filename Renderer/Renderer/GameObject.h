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
		~GameObject();
		void Init(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale);

		glm::vec3 position;
		glm::quat q;
		glm::vec3 rotation;
		glm::vec3 scale;

		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 angularVel;

		virtual void Update(float _delta);

		int texture, shader, model, material;

		glm::mat4x4 GetModelMatrix();

		void AddChild(GameObject* _child);
		std::vector<GameObject*> GetChildren();

		BoundingShape *bounding_shape;

		void SetActive(bool _isActive);
		bool IsActive();
	private:
		GameObject* parent;
		std::vector<GameObject*> children;

		bool active;
	};

}