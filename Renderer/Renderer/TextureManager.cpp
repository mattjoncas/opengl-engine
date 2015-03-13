#include "TextureManager.h"

namespace mor{

	TextureManager* TextureManager::instance = nullptr;

	TextureManager::TextureManager(){
		current_texture = -1;
	}

	TextureManager::~TextureManager(){

	}

	TextureManager& TextureManager::GetInstance(){
		if (instance == nullptr){
			instance = new TextureManager();
		}
		return *instance;
	}

	void TextureManager::BindTexture(int _index, GLenum _textureUnit){
		//texture
		if (_textureUnit == GL_TEXTURE0){
			if (_index != current_texture){
				glActiveTexture(_textureUnit);
				if (_index == -1){
					glBindTexture(GL_TEXTURE_2D, 0);
					current_texture = _index;
				}
				else if (_index < textures.size()){
					glBindTexture(GL_TEXTURE_2D, textures[_index].texture);
					current_texture = _index;
				}
				else{
					std::cout << "Texture index out of range.\n";
				}
			}
		}
		//normal map
		else if (_textureUnit == GL_TEXTURE1){
			if (_index != current_normalmap){
				glActiveTexture(_textureUnit);
				if (_index == -1){
					glBindTexture(GL_TEXTURE_2D, 0);
					current_normalmap = _index;
				}
				else if (_index < textures.size()){
					glBindTexture(GL_TEXTURE_2D, textures[_index].texture);
					current_normalmap = _index;
				}
				else{
					std::cout << "Texture index out of range. [normal map]\n";
				}
			}
		}
	}

	int TextureManager::LoadTexture(std::string _path){
		if (!CheckLoaded(_path)){
			GLuint texture;

			glGenTextures(1, &texture);

			std::string file_path = "textures/" + _path;

			glActiveTexture(GL_TEXTURE0);

			texture = SOIL_load_OGL_texture(file_path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

			glBindTexture(GL_TEXTURE_2D, texture);

			if (texture == 0){
				std::cout << "***Texture " << _path << " load failed.\n";
				return -1;
			}

			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			textures.push_back(Texture());
			textures[textures.size() - 1].Init(texture, _path);
			std::cout << _path.c_str() << " loaded.\n";

			return textures.size() - 1;
		}
		else{
			std::cout << "***Texture already loaded, index returned.\n";
			return GetIndex(_path);
		}
	}

	bool TextureManager::CheckLoaded(std::string _file){
		bool loaded = false;
		for (int i = 0; i < textures.size(); i++) {
			if (_file == textures[i].file){
				loaded = true;
				break;
			}
		}
		return loaded;
	}
	int TextureManager::GetIndex(std::string _file){
		int _index;
		for (int i = 0; i < textures.size(); i++) {
			if (_file == textures[i].file){
				_index = i;
				break;
			}
		}
		return _index;
	}

}