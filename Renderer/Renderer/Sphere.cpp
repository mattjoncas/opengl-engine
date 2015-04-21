#include "Sphere.h"


Sphere::Sphere(){

}
Sphere::Sphere(glm::vec3 _center, float _radius){
	center = _center;
	radius = _radius;
}
Sphere::~Sphere(){

}

float Sphere::Radius(){
	return radius;
}

void Sphere::SetRadius(float _r){
	radius = _r;
}

bool Sphere::Intersects(Plane _p){
	//currently need to multiply pos by -1.0f
	if (glm::dot(_p.Normal(), center * -1.0f) + _p.D() > radius){
		return true;
	}
	else{
		return false;
	}
}
bool Sphere::Intersects(Sphere _s){
	if (glm::distance(center, _s.Center()) < radius + _s.Radius()){
		return true;
	}
	else{
		return false;
	}
}

bool Sphere::Intersects(AABox _b){
	Plane front = Plane(glm::vec3(0.0f, 0.0f, 1.0f), _b.Center().z);

	if (glm::dot(front.Normal(), center) + front.D() < radius && glm::dot(front.Normal(), center) + front.D() > 0.0f){
		if (center.x < _b.Center().x + _b.Width() / 2 &&
			center.x > _b.Center().x - _b.Width() / 2 &&
			center.y < _b.Center().y + _b.Height() / 2 &&
			center.y > _b.Center().y - _b.Height() / 2){
			std::cout << "front";
			return true;
		}
	}
	Plane back = Plane(glm::vec3(0.0f, 0.0f, -1.0f), _b.Center().z);

	if (glm::dot(back.Normal(), center) + back.D() < radius && glm::dot(back.Normal(), center) + back.D() > 0.0f){
		if (center.x < _b.Center().x + _b.Width() / 2 &&
			center.x > _b.Center().x - _b.Width() / 2 &&
			center.y < _b.Center().y + _b.Height() / 2 &&
			center.y > _b.Center().y - _b.Height() / 2){
			std::cout << "back";
			return true;
		}
	}
	Plane top = Plane(glm::vec3(0.0f, 1.0f, 0.0f), _b.Center().y);

	if (glm::dot(top.Normal(), center) + top.D() < radius && glm::dot(top.Normal(), center) + top.D() > 0.0f){
		if (center.x < _b.Center().x + _b.Width() / 2 &&
			center.x > _b.Center().x - _b.Width() / 2 &&
			center.z < _b.Center().z + _b.Length() / 2 &&
			center.z > _b.Center().z - _b.Length() / 2){
			std::cout << "top";
			return true;
		}
	}
	Plane bottom = Plane(glm::vec3(0.0f, -1.0f, 0.0f), _b.Center().y);

	if (glm::dot(bottom.Normal(), center) + bottom.D() < radius && glm::dot(bottom.Normal(), center) + bottom.D() > 0.0f){
		if (center.x < _b.Center().x + _b.Width() / 2 &&
			center.x > _b.Center().x - _b.Width() / 2 &&
			center.z < _b.Center().z + _b.Length() / 2 &&
			center.z > _b.Center().z - _b.Length() / 2){
			std::cout << "bottom";
			return true;
		}
	}
	Plane left = Plane(glm::vec3(-1.0f, 0.0f, 0.0f), _b.Center().x);

	if (glm::dot(left.Normal(), center) + left.D() < radius && glm::dot(left.Normal(), center) + left.D() > 0.0f){
		if (center.y < _b.Center().y + _b.Height() / 2 &&
			center.y > _b.Center().y - _b.Height() / 2 &&
			center.z < _b.Center().z + _b.Length() / 2 &&
			center.z > _b.Center().z - _b.Length() / 2){
			std::cout << "left";
			return true;
		}
	}
	Plane right = Plane(glm::vec3(1.0f, 0.0f, 0.0f), _b.Center().x);

	if (glm::dot(right.Normal(), center) + right.D() < radius && glm::dot(right.Normal(), center) + right.D() > 0.0f){
		if (center.y < _b.Center().y + _b.Height() / 2 &&
			center.y > _b.Center().y - _b.Height() / 2 &&
			center.z < _b.Center().z + _b.Length() / 2 &&
			center.z > _b.Center().z - _b.Length() / 2){
			std::cout << "right";
			return true;
		}
	}
	return false;
}