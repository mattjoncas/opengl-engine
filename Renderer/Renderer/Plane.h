#pragma once
#include <glm/glm.hpp>

//#include "Sphere.h"

class Plane{
public:
	Plane();
	Plane(glm::vec3 _normal, float _d);
	~Plane();

	glm::vec3 Normal();
	float D();

	void SetNormal(glm::vec3 _n);
	void SetD(float _d);

	//bool Intersects(Sphere _s);
private:
	glm::vec3 normal;
	float d;
};

