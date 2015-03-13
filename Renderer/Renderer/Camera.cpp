#include "Camera.h"

namespace mor{
	Camera::Camera(){
		pos = glm::vec3(0.0f, 0.0f, 5.0f);
		dir = glm::vec3(0.0f, 0.0f, -1.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);

		fov = 45.0f;
		fnear = 0.1f;
		ffar = 200.0f;

		fps = false;

		UpdateFrustum();
	}
	Camera::Camera(glm::vec3 _pos, glm::vec3 _dir, glm::vec3 _up, int _screenW, int _screenH, bool _fps){
		pos = _pos;
		dir = _dir;
		up = _up;
		sWidth = _screenW;
		sHeight = _screenH;

		fov = 45.0f;
		fnear = 0.1f;
		ffar = 200.0f;

		fps = _fps;

		UpdateFrustum();
	}

	Camera::~Camera(){

	}
	glm::mat4x4 Camera::GetView(){
		view = glm::lookAt(pos, pos + dir, up);
		return view;
	}
	glm::mat4x4 Camera::GetProj(){
		proj = glm::perspective(fov, float(sWidth) / float(sHeight), fnear, ffar);
		return proj;
	}
	void Camera::SetPosition(glm::vec3 _newPos){
		pos = _newPos;
	}
	void Camera::SetScreenSize(int _width, int _height){
		sWidth = _width;
		sHeight = _height;
	}

	void Camera::Update(float _delta){
		if (fps){
			//temp vec3 pos to throttle movement
			glm::vec3 disp(0.0f, 0.0f, 0.0f);
			//translate forward using pos2[aka look direction vector]
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
				disp.x += dir.x;
				disp.z += dir.z;
			}
			//translate back
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
				disp.x -= dir.x;
				disp.z -= dir.z;
			}
			//translate left
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
				disp.x -= glm::cross(dir, up).x;
				disp.z -= glm::cross(dir, up).z;
			}
			//translate right
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
				disp.x += glm::cross(dir, up).x;
				disp.z += glm::cross(dir, up).z;
			}
			//translate up y axis at fixed rate
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
				disp.y += 1;
			}
			//translate down y
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
				disp.y -= 1;
			}
			if (disp != glm::vec3(0, 0, 0)){
				pos += glm::normalize(disp) * _delta * CAMERA_SPEED;
			}

			//update direction vector [which rotates camera]
			if (sf::Mouse::getPosition() != sf::Vector2i(sWidth / 2, sHeight / 2)){
				glm::vec2 mouseDelta = glm::vec2(sWidth / 2 - sf::Mouse::getPosition().x, sHeight / 2 - sf::Mouse::getPosition().y);
				glm::mat4 rot;
				//update direction vector y rotation [left/right mouse movement]
				rot = glm::rotate(rot, mouseDelta.x * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
				//update direction vector z/x rotation using cross product of dir/up vectors [up/down mouse movement]
				rot = glm::rotate(rot, mouseDelta.y * 0.1f, glm::vec3(1.0f * glm::cross(dir, up).x, 0.0f, 1.0f * glm::cross(dir, up).z));

				dir = glm::vec3(rot * glm::vec4(dir.x, dir.y, dir.z, 1.0f));

				//keep mouse at center so rotation isn't locked
				sf::Mouse::setPosition(sf::Vector2i(sWidth / 2, sHeight / 2));
			}
		}
		if (lerping){
			lerp_time += _delta;
			if (lerp_time >= lerp_end_time){
				pos = lerp_end;
				lerping = false;
			}
			else{
				float c_time = lerp_time / lerp_end_time;
				pos = lerp_start + c_time * (lerp_end - lerp_start);
			}
		}
		
		UpdateFrustum();
	}
	bool Camera::InFrustum(glm::vec3 _pos){
		//no idea why this is needed vv
		glm::vec3 newpos = _pos * -1.0f;

		//check if position is within the frustum planes
		if (glm::dot(right.Normal(), newpos) + right.D() < 0 &&
			glm::dot(left.Normal(), newpos) + left.D() < 0 &&
			glm::dot(bot.Normal(), newpos) + bot.D() < 0 &&
			glm::dot(top.Normal(), newpos) + top.D() < 0){
			return true;
		}
		else{
			return false;
		}
	}
	bool Camera::InFrustum(glm::mat4x4 _modelMatrix){
		//get frustum from model matrix
		glm::vec3 newpos = glm::vec3(_modelMatrix[3][0], _modelMatrix[3][1], _modelMatrix[3][2]) * -1.0f; //not sure why position needs to be negated

		//check if position is outside the frustum
		if (glm::dot(right.Normal(), newpos) + right.D() > 0){
			return false;
		}
		if (glm::dot(left.Normal(), newpos) + left.D() > 0){
			return false;
		}
		if (glm::dot(bot.Normal(), newpos) + bot.D() > 0){
			return false;
		}
		if (glm::dot(top.Normal(), newpos) + top.D() > 0){
			return false;
		}
		return true;
	}
	bool Camera::InFrustum(BoundingShape *_shape){
		if (Sphere * s = dynamic_cast<Sphere*>(_shape)) {
			return InFrustum(s);
		}
		else if (AABox * b = dynamic_cast<AABox*>(_shape)) {
			return InFrustum(b);
		}
		else{
			return false;
		}
	}
	bool Camera::InFrustum(Sphere *_sphere){
		//check if position is outside the frustum
		if (_sphere->Intersects(right)){
			return false;
		}
		if (_sphere->Intersects(left)){
			return false;
		}
		if (_sphere->Intersects(bot)){
			return false;
		}
		if (_sphere->Intersects(top)){
			return false;
		}
		return true;
	}
	bool Camera::InFrustum(AABox *_box){
		if (_box->Intersects(right)){
			return false;
		}
		if (_box->Intersects(left)){
			return false;
		}
		if (_box->Intersects(bot)){
			return false;
		}
		if (_box->Intersects(top)){
			return false;
		}
		return true;
	}
	void Camera::UpdateFrustum(){
		float hFar = 2 * tan(glm::radians(fov) / 2) * ffar;
		float wFar = hFar * sWidth / sHeight;

		//near center
		glm::vec3 nc = pos + dir * ffar;

		//right plane
		glm::vec3 r = (nc + glm::cross(dir, up) * wFar / 2.0f) - pos;
		r = glm::normalize(r);
		right.SetNormal(glm::cross(up, r));
		right.SetD(glm::dot(right.Normal(), pos));

		//left plane
		glm::vec3 l = (nc - glm::cross(dir, up) * wFar / 2.0f) - pos;
		l = glm::normalize(l);
		left.SetNormal(glm::cross(l, up));
		left.SetD(glm::dot(left.Normal(), pos));

		//bottom plane
		glm::vec3 b = (nc - up * hFar / 2.0f) - pos;
		b = glm::normalize(b);
		bot.SetNormal(glm::cross(glm::cross(dir, up), b));
		bot.SetD(glm::dot(bot.Normal(), pos));

		//top plane
		glm::vec3 t = (nc + up * hFar / 2.0f) - pos;
		t = glm::normalize(t);
		top.SetNormal(glm::cross(t, glm::cross(dir, up)));
		top.SetD(glm::dot(top.Normal(), pos));
	}
	void Camera::Lerp(glm::vec3 target_position, float _lerp_time){
		lerping = true;
		lerp_start = pos;
		lerp_end = target_position;
		lerp_time = 0.0f;
		lerp_end_time = _lerp_time;
	}
}