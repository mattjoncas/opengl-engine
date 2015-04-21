#pragma once
#include <glm/glm.hpp>
#include <iostream>

#include "BoundingShape.h"
#include "AABox.h"

class Sphere : public BoundingShape{
public:
	Sphere();
	Sphere(glm::vec3 _center, float _radius);
	~Sphere();

	float Radius();

	void SetCenter(glm::vec3 _c);
	void SetRadius(float _r);

	virtual bool Intersects(Plane _p);
	bool Intersects(Sphere _s);
	bool Intersects(AABox _b);
private:
	float radius;
};

