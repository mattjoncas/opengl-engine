#pragma once

#include <glm/glm.hpp>
#include <iostream>

#include "Plane.h"
#include "Sphere.h"
#include "AABox.h"

class Ray{
public:
	Ray();
	Ray(glm::vec3 _origin, glm::vec3 _dir, float _length);
	~Ray();

	glm::vec3 Origin();
	glm::vec3 Direction();

	bool Intersects(Plane _p);
	bool Intersects(Sphere _s);
	bool Intersects(AABox _b);

	glm::vec3 Hit();
private:
	glm::vec3 origin;
	glm::vec3 direction;
	float length;

	glm::vec3 hit;
};

