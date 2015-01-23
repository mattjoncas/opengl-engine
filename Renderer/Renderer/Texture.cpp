#include "Texture.h"

namespace mor{

	Texture::Texture(){

	}

	Texture::~Texture(){

	}

	void Texture::Init(GLuint _texture, std::string _file){
		texture = _texture;
		file = _file;
	}
}