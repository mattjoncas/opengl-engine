#pragma once

#include <glm/glm.hpp>

#include "Plane.h"

class BoundingShape{
public:
	BoundingShape();
	~BoundingShape();

	glm::vec3 Center();
	void SetCenter(glm::vec3 _c);

	virtual bool Intersects(Plane _p);

protected:
	glm::vec3 center;
};

