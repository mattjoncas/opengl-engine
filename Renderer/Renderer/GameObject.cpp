#include "GameObject.h"

namespace mor{

	GameObject::GameObject(){
		position = glm::vec3(0.0f);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
		velocity = glm::vec3(0.0f);
		acceleration = glm::vec3(0.0f);
		angularVel = glm::vec3(0.0f);

		model = -1;
		texture = -1;
		shader = 0;
		material = 0;
		normal_map = -1;

		active = true;
	}
	GameObject::GameObject(glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale, glm::vec3 _vel, glm::vec3 _accel, glm::vec3 _angVel){
		position = _pos;
		rotation = _rot;
		q = glm::quat(_rot);
		scale = _scale;
		velocity = _vel;
		acceleration = _accel;
		angularVel = _angVel;

		model = -1;
		texture = -1;
		shader = 0;
		material = 0;
		normal_map = -1;

		active = true;
		CreateBoundingSphere();
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

		CreateBoundingSphere();
	}
	void GameObject::Init(int _model, int _shader, int _texture, int _material){
		if (_model){
			model = _model;
		}
		if (_shader){
			shader = _shader;
		}
		if (_texture){
			texture = _texture;
		}
		if (_material){
			material = _material;
		}
	}

	glm::mat4x4 GameObject::GetModelMatrix(){
		glm::mat4x4 _model;

		//translate
		_model = glm::translate(_model, position);
		
		//rotation
		_model *= glm::mat4_cast(q);
		
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
	GameObject* GameObject::GetChild(int _index){
		return children[_index];
	}

	void GameObject::CreateBoundingSphere(){
		float r = scale.x;
		if (scale.y > r){
			r = scale.y;
		}
		if (scale.z > r){
			r = scale.z;
		}
		r /= 2;
		
		bounding_shape = dynamic_cast<Sphere*>(new Sphere(glm::vec3(GetModelMatrix()[3][0], GetModelMatrix()[3][1], GetModelMatrix()[3][2]), r));
	}
}