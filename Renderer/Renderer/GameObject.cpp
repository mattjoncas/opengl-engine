#include "GameObject.h"

namespace mor{

	GameObject::GameObject(){
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		angularVel = glm::vec3(0.0f, 0.0f, 0.0f);

		model = -1;
		texture = -1;
		shader = 0;
		material = 0;

		active = true;
	}

	GameObject::~GameObject(){
		for (std::vector<mor::GameObject*>::iterator iter = children.begin(); iter != children.end(); ++iter){
			delete((*iter));
		}
		delete(bounding_shape);
	}

	void GameObject::Init(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale){
		position = _pos;
		rotation = _rot;
		q = glm::quat(_rot);
		scale = _scale;

		float r = scale.x;
		if (scale.y > r){
			r = scale.y;
		}
		if (scale.z > r){
			r = scale.z;
		}
		r /= 2;

		//update sphere
		bounding_shape = dynamic_cast<Sphere*>(new Sphere(glm::vec3(GetModelMatrix()[3][0], GetModelMatrix()[3][1], GetModelMatrix()[3][2]), r));
	}

	glm::mat4x4 GameObject::GetModelMatrix(){
		glm::mat4x4 _model;

		//translate
		_model = glm::translate(_model, position);
		
		//rotation
		_model *= glm::mat4_cast(q);
		//_model = glm::rotate(_model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		//_model = glm::rotate(_model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		//_model = glm::rotate(_model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		//scale
		_model = glm::scale(_model, scale);

		if (parent){
			_model = parent->GetModelMatrix() * _model;
		}

		return _model;
	}

	void GameObject::Update(float _delta){
		if (active){
			//update pos
			position = position + velocity * _delta + acceleration / 2.0f * (_delta * _delta);

			//update vel
			velocity = velocity + acceleration * _delta;

			//update quaternion with angular vel
			{
				glm::vec3 ha = angularVel * _delta / 2.0f; // vector of half angle
				double l = ha.length(); // magnitude
				glm::quat dq;
				if (l > 0)
				{
					double ss = sin(l) / l;
					dq = glm::quat(cos(l), ha.x * ss, ha.y * ss, ha.z * ss);
				}
				else{
					dq = glm::quat(1.0, ha.x, ha.y, ha.z);
				}
				q *= dq;
				q = glm::normalize(q);
			}
			//q = glm::rotate(q, angularVel.x * _delta, glm::vec3(1.0f, 0.0f, 0.0f));
			//q = glm::rotate(q, angularVel.y * _delta, glm::vec3(0.0f, 1.0f, 0.0f));
			//q = glm::rotate(q, angularVel.z * _delta, glm::vec3(0.0f, 0.0f, 1.0f));
			rotation += angularVel * _delta;

			//update children
			for (int i = 0; i < children.size(); i++){
				children[i]->Update(_delta);
			}
		}
	}
	void GameObject::AddChild(GameObject* _child){
		_child->parent = this;
		children.push_back(_child);
	}
	std::vector<GameObject*> GameObject::GetChildren(){
		return children;
	}

	void GameObject::SetActive(bool _isActive){
		active = _isActive;
	}
	bool GameObject::IsActive(){
		return active;
	}
}