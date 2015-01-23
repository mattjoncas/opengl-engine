#include "Plane.h"


Plane::Plane(){

}
Plane::Plane(glm::vec3 _normal, float _d){
	normal = _normal;
	d = _d;
}
Plane::~Plane(){

}

glm::vec3 Plane::Normal(){
	return normal;
}
float Plane::D(){
	return d;
}

void Plane::SetNormal(glm::vec3 _n){
	normal = _n;
}
void Plane::SetD(float _d){
	d = _d;
}
/*
bool Plane::Intersects(Sphere _s){
	//currently need to multiply pos by -1.0f
	if (glm::dot(normal, _s.Center() * -1.0f) + d > _s.Radius()){
		return true;
	}
	else{
		return false;
	}
}*/