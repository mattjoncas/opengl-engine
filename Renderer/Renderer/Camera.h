#pragma once
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Plane.h"
#include "BoundingShape.h"
#include "Ray.h"

#define CAMERA_SPEED 20.0f

namespace mor{
	class Camera
	{
	public:
		Camera();
		Camera(glm::vec3 _pos, glm::vec3 _dir, glm::vec3 _up, int _screenW, int _screenH, bool _fps);
		~Camera();

		//"camera" position
		glm::vec3 pos;
		//direction vector
		glm::vec3 dir;
		//up vector
		glm::vec3 up;

		void Update(float _delta);
		void UpdateFrustum();
		void SetPosition(glm::vec3 _newPos);
		void SetScreenSize(int _width, int _height);
		glm::mat4x4 GetView();
		glm::mat4x4 GetProj();

		bool InFrustum(glm::vec3 _pos);
		bool InFrustum(glm::mat4x4 _modelMatrix);
		bool InFrustum(BoundingShape *_shape);

		//made public for shadow testing
		float fov, fnear, ffar;

		void Lerp(glm::vec3 target_position, float _lerp_time);
	private:
		int sWidth, sHeight;
		

		glm::mat4x4 view;
		glm::mat4x4 proj;

		Plane top, bot, left, right;

		bool InFrustum(Sphere *_sphere);
		bool InFrustum(AABox *_box);

		bool fps;

		bool lerping = false;
		glm::vec3 lerp_start, lerp_end;
		float lerp_time, lerp_end_time;
	};

}