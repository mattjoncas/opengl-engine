#include "Ray.h"


Ray::Ray(){

}
Ray::Ray(glm::vec3 _origin, glm::vec3 _dir, float _length){
	origin = _origin;
	direction = _dir;
	length = _length;
}

Ray::~Ray(){

}

glm::vec3 Ray::Origin(){
	return origin;
}
glm::vec3 Ray::Direction(){
	return direction;
}

bool Ray::Intersects(Plane _p){
	float denom = glm::dot(_p.Normal(), direction);
	if (denom < 0.0000001) {
		glm::vec3 p0l0 = _p.Normal() * _p.D() - origin;
		float t = glm::dot(p0l0, _p.Normal()) / denom;
		if (t >= 0){
			//find ray hit
			glm::vec3 l = direction;
			glm::vec3 l0 = origin;
			l *= t;
			l += l0;
			hit = l;
			return true;

		}
	}
	return false;
}
bool Ray::Intersects(Sphere _s){
	glm::vec3 u = _s.Center() - origin;
	
	//my way
	//multiply direction normal by difference mag
	glm::vec3 puv = direction * glm::length(u);
	//add origin vector
	glm::vec3 q = origin + puv;
	
	//[way i found online] -->this returns true when facing away from a sphere
	//glm::vec3 puv = glm::dot(direction, u) / glm::length(direction) * direction;
	//glm::vec3 q = origin + puv;

	if (glm::length(u) - _s.Radius() < length || length == 0.0f){
		float dist = glm::length(_s.Center() - q);

		if (dist < _s.Radius()){
			return true;
		}
	}
	return false;
}
bool Ray::Intersects(AABox _b){
	Plane front = Plane(glm::vec3(0.0f, 0.0f, 1.0f), _b.Center().z);

	float denom = glm::dot(front.Normal(), direction);
	if (denom < 0.0000001) {
		// cube face normal * half scale + the offsets of the entire cube and ray
		glm::vec3 p0l0 = front.Normal() * (_b.Length() / 2) + front.D() - origin;
		float t = glm::dot(p0l0, front.Normal()) / denom;
		if (t >= 0 && (length > t || length == 0.0f)){
			glm::vec3 l = direction;
			glm::vec3 l0 = origin;
			//find ray hit pos
			l *= t;
			l += l0;
			
			if (l.x < _b.Center().x + _b.Width() / 2 &&
				l.x > _b.Center().x - _b.Width() / 2 &&
				l.y < _b.Center().y + _b.Height() / 2 &&
				l.y > _b.Center().y - _b.Height() / 2){
				//std::cout << "front hit";
				hit = l;
				return true;
			}
		}
	}
	Plane back = Plane(glm::vec3(0.0f, 0.0f, -1.0f), _b.Center().z);

	denom = glm::dot(back.Normal(), direction);
	if (denom < 0.0000001) {
		glm::vec3 p0l0 = back.Normal() * (_b.Length() / 2) + back.D() - origin;
		float t = glm::dot(p0l0, back.Normal()) / denom;
		if (t >= 0 && (length > t || length == 0.0f)){
			glm::vec3 l = direction;
			glm::vec3 l0 = origin;
			//find ray hit pos
			l *= t;
			l += l0;

			if (l.x < _b.Center().x + _b.Width() / 2 &&
				l.x > _b.Center().x - _b.Width() / 2 &&
				l.y < _b.Center().y + _b.Height() / 2 &&
				l.y > _b.Center().y - _b.Height() / 2){
				//std::cout << "back hit";
				hit = l;
				return true;
			}
		}
	}
	Plane top = Plane(glm::vec3(0.0f, 1.0f, 0.0f), _b.Center().y);

	denom = glm::dot(top.Normal(), direction);
	if (denom < 0.0000001) {
		glm::vec3 p0l0 = top.Normal() * (_b.Height() / 2) + top.D() - origin;
		float t = glm::dot(p0l0, top.Normal()) / denom;
		if (t >= 0 && (length > t || length == 0.0f)){
			glm::vec3 l = direction;
			glm::vec3 l0 = origin;
			//find ray hit pos
			l *= t;
			l += l0;
			
			if (l.x < _b.Center().x + _b.Width() / 2 &&
				l.x > _b.Center().x - _b.Width() / 2 &&
				l.z < _b.Center().z + _b.Length() / 2 &&
				l.z > _b.Center().z - _b.Length() / 2){
				//std::cout << "top hit";
				hit = l;
				return true;
			}
		}
	}
	Plane bottom = Plane(glm::vec3(0.0f, -1.0f, 0.0f), _b.Center().y);

	denom = glm::dot(bottom.Normal(), direction);
	if (denom < 0.0000001) {
		glm::vec3 p0l0 = bottom.Normal() * (_b.Height() / 2) + bottom.D() - origin;
		float t = glm::dot(p0l0, bottom.Normal()) / denom;
		if (t >= 0 && (length > t || length == 0.0f)){
			glm::vec3 l = direction;
			glm::vec3 l0 = origin;
			//find ray hit pos
			l *= t;
			l += l0;

			if (l.x < _b.Center().x + _b.Width() / 2 &&
				l.x > _b.Center().x - _b.Width() / 2 &&
				l.z < _b.Center().z + _b.Length() / 2 &&
				l.z > _b.Center().z - _b.Length() / 2){
				//std::cout << "bot hit";
				hit = l;
				return true;
			}
		}
	}
	Plane left = Plane(glm::vec3(-1.0f, 0.0f, 0.0f), _b.Center().x);

	denom = glm::dot(left.Normal(), direction);
	if (denom < 0.0000001) {
		glm::vec3 p0l0 = left.Normal() * (_b.Width() / 2) + left.D() - origin;
		float t = glm::dot(p0l0, left.Normal()) / denom;
		if (t >= 0 && (length > t || length == 0.0f)){
			glm::vec3 l = direction;
			glm::vec3 l0 = origin;
			//find ray hit pos
			l *= t;
			l += l0;

			if (l.y < _b.Center().y + _b.Height() / 2 &&
				l.y > _b.Center().y - _b.Height() / 2 &&
				l.z < _b.Center().z + _b.Length() / 2 &&
				l.z > _b.Center().z - _b.Length() / 2){
				//std::cout << "left hit";
				hit = l;
				return true;
			}
		}
	}
	Plane right = Plane(glm::vec3(1.0f, 0.0f, 0.0f), _b.Center().x);

	denom = glm::dot(right.Normal(), direction);
	if (denom < 0.0000001) {
		glm::vec3 p0l0 = right.Normal() * (_b.Width() / 2) + right.D() - origin;
		float t = glm::dot(p0l0, right.Normal()) / denom;
		if (t >= 0 && (length > t || length == 0.0f)){
			glm::vec3 l = direction;
			glm::vec3 l0 = origin;
			//find ray hit pos
			l *= t;
			l += l0;

			if (l.y < _b.Center().y + _b.Height() / 2 &&
				l.y > _b.Center().y - _b.Height() / 2 &&
				l.z < _b.Center().z + _b.Length() / 2 &&
				l.z > _b.Center().z - _b.Length() / 2){
				//std::cout << "right hit";
				hit = l;
				return true;
			}
		}
	}
	return false;
}

glm::vec3 Ray::Hit(){
	return hit;
}