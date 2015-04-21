#pragma once
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>
#include <string>

namespace mor{

	class Texture
	{
	public:
		Texture();
		~Texture();

		void Init(GLuint _texture, std::string _file);

		GLuint texture;
		std::string file;
	};

}