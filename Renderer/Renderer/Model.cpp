#include "Model.h"

namespace mor{

	Model::Model(){

	}

	Model::~Model(){

	}

	void Model::Init(std::vector<GLfloat> _v, std::vector<GLuint> _e, std::string _name){
		name = _name;
		count = _e.size();

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, _v.size() * sizeof(GLfloat), &_v[0], GL_STATIC_DRAW);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _e.size() * sizeof(GLuint), &_e[0], GL_STATIC_DRAW);
	}

}