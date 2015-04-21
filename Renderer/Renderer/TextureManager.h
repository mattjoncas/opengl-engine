#pragma once
#include "Texture.h"
#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>
#include "SOIL.h"

namespace mor{

	class TextureManager{
	private:
		static TextureManager *instance;
		std::vector<Texture> textures;

		int current_texture, current_normalmap;

		TextureManager();
		bool CheckLoaded(std::string _file);
		int GetIndex(std::string _file);
	public:
		~TextureManager();

		static TextureManager& GetInstance();
		//load a texture, returns its index in vector
		int LoadTexture(std::string _path);
		//bind shader by its index
		void BindTexture(int _index, GLenum _textureUnit);
	};

}