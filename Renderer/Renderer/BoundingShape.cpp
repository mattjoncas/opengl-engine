#include "BoundingShape.h"


BoundingShape::BoundingShape(){

}


BoundingShape::~BoundingShape(){

}

glm::vec3 BoundingShape::Center(){
	return center;
}
void BoundingShape::SetCenter(glm::vec3 _c){
	center = _c;
}

bool BoundingShape::Intersects(Plane _p){
	return false;
}