#include "Shader.h"

namespace mor{

	Shader::Shader(){

	}

	Shader::~Shader(){
		glDeleteProgram(shader);
	}
	//returns GLuint which you use to bind
	GLuint Shader::GetProgram(){
		return shader;
	}

	void Shader::Init(GLuint _program, std::string _name){
		shader = _program;
		name = _name;
		InitAttributes();
	}

	void Shader::InitAttributes(){
		//set up shader attributes [links position attribute to vertices array]
		posAttrib = glGetAttribLocation(shader, "position");
		glEnableVertexAttribArray(posAttrib);
		//glVertexAttribBinding(posAttrib, 0);
		//glVertexAttribFormat(posAttrib, 3, GL_FLOAT, GL_FALSE, 0);
		//normals
		norAttrib = glGetAttribLocation(shader, "normal");
		glEnableVertexAttribArray(norAttrib);
		//glVertexAttribBinding(norAttrib, 1);
		//glVertexAttribFormat(norAttrib, 3, GL_FLOAT, GL_FALSE, 0);
		//links texture coord to vertiecs array
		texAttrib = glGetAttribLocation(shader, "texcoord");
		glEnableVertexAttribArray(texAttrib);
		//glVertexAttribBinding(texAttrib, 3);
		//glVertexAttribFormat(texAttrib, 2, GL_FLOAT, GL_FALSE, 0);
		//[links color attribute to vertices array]
		colAttrib = glGetAttribLocation(shader, "color");
		glEnableVertexAttribArray(colAttrib);
		//glVertexAttribBinding(colAttrib, 2);
		//glVertexAttribFormat(colAttrib, 3, GL_FLOAT, GL_FALSE, 0);

		uniModel = glGetUniformLocation(shader, "model");
		uniNormal = glGetUniformLocation(shader, "norm");

		uniNumLights = glGetUniformLocation(shader, "num_lights");
	}
	void Shader::Enable(){
		
		//glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), 0);
		glEnableVertexAttribArray(posAttrib);
		
		//glVertexAttribPointer(norAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(norAttrib);
		
		//glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(texAttrib);

		//glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (void*)(8 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(colAttrib);
	}
}