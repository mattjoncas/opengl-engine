#include "LightManager.h"

namespace mor{

	LightManager* LightManager::instance = nullptr;

	LightManager::LightManager(){

	}

	LightManager::~LightManager(){
		for (std::vector<mor::Light*>::iterator iter = lights.begin(); iter != lights.end(); ++iter){
			delete((*iter));
		}
	}

	LightManager& LightManager::GetInstance(){
		if (instance == nullptr){
			instance = new LightManager();
		}
		return *instance;
	}

	void LightManager::AddLight(Light* _newLight){
		if (lights.size() < MAX_LIGHTS){
			lights.push_back(_newLight);
			std::cout << "New light added to LightManager.\n";
		}
	}
	void LightManager::RemoveLight(Light* _light){
		for (std::vector<mor::Light*>::iterator iter = lights.begin(); iter != lights.end(); ++iter){
			if (*iter == _light){
				lights.erase(iter);
				break;
			}
		}
	}
	void LightManager::UpdateUniforms(GLuint _light_ubo){
		glBindBuffer(GL_UNIFORM_BUFFER, _light_ubo);
		for (int i = 0; i < lights.size(); i++){
			if (lights[i]->UpdateCheck()){
				int ubo_offset = sizeof(glm::vec4) * 4 + sizeof(float) * 2;// light ubo byte size
				ubo_offset *= i;
				glBufferSubData(GL_UNIFORM_BUFFER, ubo_offset, sizeof(glm::vec4), glm::value_ptr(lights[i]->Position()));
				glBufferSubData(GL_UNIFORM_BUFFER, ubo_offset + sizeof(glm::vec4) * 4, sizeof(float), &lights[i]->size);
				glBufferSubData(GL_UNIFORM_BUFFER, ubo_offset + sizeof(glm::vec4) * 4 + sizeof(float), sizeof(float), &lights[i]->drop_off_rate);
				if (lights[i]->IsOn()){
					glBufferSubData(GL_UNIFORM_BUFFER, ubo_offset + sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(lights[i]->Ambient()));
					glBufferSubData(GL_UNIFORM_BUFFER, ubo_offset + sizeof(glm::vec4) * 2, sizeof(glm::vec4), glm::value_ptr(lights[i]->Diffuse()));
					glBufferSubData(GL_UNIFORM_BUFFER, ubo_offset + sizeof(glm::vec4) * 3, sizeof(glm::vec4), glm::value_ptr(lights[i]->Specular()));
				}
				else{
					glBufferSubData(GL_UNIFORM_BUFFER, ubo_offset + sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(glm::vec4(0.0f)));
					glBufferSubData(GL_UNIFORM_BUFFER, ubo_offset + sizeof(glm::vec4) * 2, sizeof(glm::vec4), glm::value_ptr(glm::vec4(0.0f)));
					glBufferSubData(GL_UNIFORM_BUFFER, ubo_offset + sizeof(glm::vec4) * 3, sizeof(glm::vec4), glm::value_ptr(glm::vec4(0.0f)));
				}
			}
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	int LightManager::LightCount(){
		return lights.size();
	}

	glm::vec3 LightManager::GetLightPosition(int _index){
		return glm::vec3(lights[_index]->Position());
	}
}