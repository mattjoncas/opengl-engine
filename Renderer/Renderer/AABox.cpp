#include "AABox.h"


AABox::AABox(){

}
AABox::AABox(glm::vec3 _center, float _w, float _h, float _l){
	center = _center;
	width = _w;
	height = _h;
	length = _l;

	UpdateCorners();
}
AABox::~AABox(){

}

float AABox::Width(){
	return width;
}
float AABox::Height(){
	return height;
}
float AABox::Length(){
	return length;
}

void AABox::SetWidth(float _w){
	width = _w;
}
void AABox::SetHeight(float _h){
	height = _h;
}
void AABox::SetLength(float _l){
	length = _l;
}

bool AABox::Intersects(Plane _p){
	for (int i = 0; i < 8; i++){
		//if a single corner is behind the plane it returns false
		if (glm::dot(_p.Normal(), -corners[i]) + _p.D() < 0){
			return false;
		}
	}
	return true;
}
bool AABox::Intersects(AABox _b){
	if (abs(center.z - _b.center.z) >= length / 2 + _b.Length() / 2){
		return false;
	}
	else if (abs(center.x - _b.center.x) >= width / 2 + _b.Width() / 2){
		return false;
	}
	else if (abs(center.y - _b.center.y) >= height / 2 + _b.Height() / 2){
		return false;
	}
	return true;
}

void AABox::UpdateCorners(){
	glm::vec3 c = center;
	c.x += width;
	c.y += height;
	c.z += length;
	corners[0] = c;
	c = center;
	c.x += width;
	c.y += height;
	c.z -= length;
	corners[1] = c;
	c = center;
	c.x -= width;
	c.y += height;
	c.z -= length;
	corners[2] = c;
	c = center;
	c.x -= width;
	c.y += height;
	c.z += length;
	corners[3] = c;
	c.x += width;
	c.y -= height;
	c.z += length;
	corners[4] = c;
	c = center;
	c.x += width;
	c.y -= height;
	c.z -= length;
	corners[5] = c;
	c = center;
	c.x -= width;
	c.y -= height;
	c.z -= length;
	corners[6] = c;
	c = center;
	c.x -= width;
	c.y -= height;
	c.z += length;
	corners[7] = c;
}