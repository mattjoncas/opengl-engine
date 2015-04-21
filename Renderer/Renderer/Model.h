#pragma once
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace mor{

	class Model
	{
	public:
		Model();
		~Model();
		//stride is used to calculate the amount of vertices
		void Init(std::vector<GLfloat> _v, std::vector<GLuint> _e, std::string _name);
		int count;
		std::string name;

		GLuint vbo, ebo;

		std::vector<int> vert_count;
	};

}