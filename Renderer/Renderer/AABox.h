#pragma once

#include "BoundingShape.h"

class AABox : public BoundingShape{
public:
	AABox();
	AABox(glm::vec3 _center, float _w, float _h, float _l);
	~AABox();

	float Width();
	float Height();
	float Length();

	void SetWidth(float _w);
	void SetHeight(float _h);
	void SetLength(float _l);

	virtual bool Intersects(Plane _p);
	bool Intersects(AABox);
private:
	float width, height, length;
	
	glm::vec3 corners[8];

	void UpdateCorners();
};

