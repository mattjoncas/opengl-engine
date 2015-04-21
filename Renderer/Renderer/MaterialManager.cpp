#include "MaterialManager.h"

namespace mor{

	MaterialManager* MaterialManager::instance = nullptr;

	MaterialManager::MaterialManager(){

	}

	MaterialManager::~MaterialManager(){

	}

	MaterialManager& MaterialManager::GetInstance(){
		if (instance == nullptr){
			instance = new MaterialManager();
		}
		return *instance;
	}

	int MaterialManager::AddMaterial(glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular, float _shininess){
		materials.push_back(Material(_ambient, _diffuse, _specular, _shininess));

		return materials.size() - 1;
	}

	void MaterialManager::BindMaterial(int _index, GLuint _ubo){
		if (_index != current){
			if (_index < materials.size()){

				glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(materials[_index].material_ambient));
				glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(materials[_index].material_diffuse));
				glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 2, sizeof(glm::vec4), glm::value_ptr(materials[_index].material_specular));
				glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 3, sizeof(float), &materials[_index].material_shininess);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}
			else{
				std::cout << "Material index out of range.\n";
			}
		}
	}
}