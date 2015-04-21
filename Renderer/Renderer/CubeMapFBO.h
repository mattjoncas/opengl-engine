#pragma once
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <fstream>

class CubeMapFBO{

public:
	CubeMapFBO();
	~CubeMapFBO();

	void Init(int window_width, int window_height); //some people just use 1024x1024

	void BindForWriting(GLenum cube_face);
	void BindForReading(GLenum TextureUnit);
private:
	GLuint fbo, depth_rb, cubeMap;
};