#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>

namespace mor{

	class Shader{
	public:
		Shader();
		~Shader();
		//init shader, pass in a loaded program
		void Init(GLuint _program, std::string _name);
		//returns program
		GLuint GetProgram();

		GLuint uniModel, uniNormal, uniNumLights;

		void Enable();

		std::string name;
	private:
		GLuint posAttrib, norAttrib, colAttrib, texAttrib;

		//shader unsigned int
		GLuint shader;
		//inits ttributes once program has been loaded
		void InitAttributes();
	};

}