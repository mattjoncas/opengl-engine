#include "ModelManager.h"

namespace mor{

	ModelManager* ModelManager::instance = nullptr;

	ModelManager::ModelManager(){

	}

	ModelManager::~ModelManager(){

	}

	ModelManager& ModelManager::GetInstance(){
		if (instance == nullptr){
			instance = new ModelManager();
		}
		return *instance;
	}

	int ModelManager::LoadModel(std::string _model){
		if (!CheckLoaded(_model)){
			std::vector<GLfloat> _v;
			std::vector<GLuint> _e;

			std::string file_path = "models/";

			file_path += _model + ".ply";
			
			std::ifstream model_input;
			model_input.open(file_path.c_str());
			std::string data;
			int v_count = 0;

			bool _header = true;
			while (_header){
				model_input >> data;
				
				if (data == "element"){
					model_input >> data;
					if (data == "vertex"){
						model_input >> v_count;
					}
				}
				if (data == "end_header"){
					_header = false;
				}
			}

			float a;
			//load vbo
			int c = 0;
			while (v_count > 0){
				model_input >> a;
				c++;
				if (c == 9 || c == 10 || c == 11){ a /= 255; }
				if (c == 11){ c = 0; v_count--; }
				_v.push_back(a);
			}
			_v.push_back(a); //not sure why i need this << [last vertex loses its color if not]

			//load ebo
			c = 0;
			std::vector<int> _vert_count;
			while (model_input >> a){
				if (c == 0){
					_vert_count.push_back(a);
					c = a;
				}
				else{
					_e.push_back(a);
					c--;
				}
			}

			models.push_back(Model());
			models[models.size() - 1].Init(_v, _e, _model);
			models[models.size() - 1].vert_count = _vert_count;

			std::cout << _model.c_str() << " model loaded.\n";

			return models.size() - 1;
		}
		else{
			std::cout << "***Model already loaded, index returned.\n";
			return GetIndex(_model);
		}
	}
	void ModelManager::BindModel(int _index){
		if (_index != current){
			if (_index < models.size()){
				current = _index;
				glBindVertexBuffer(0, models[_index].vbo, 0, 11 * sizeof(GLfloat));
				glBindVertexBuffer(1, models[_index].vbo, 3 * sizeof(GLfloat), 11 * sizeof(GLfloat));
				glBindVertexBuffer(2, models[_index].vbo, 6 * sizeof(GLfloat), 11 * sizeof(GLfloat));
				glBindVertexBuffer(3, models[_index].vbo, 8 * sizeof(GLfloat), 11 * sizeof(GLfloat));

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[_index].ebo);
			}
			else{
				std::cout << "Model index out of range.\n";
			}
		}
	}

	bool ModelManager::CheckLoaded(std::string _name){
		bool loaded = false;
		for (int i = 0; i < models.size(); i++) {
			if (_name == models[i].name){
				loaded = true;
				break;
			}
		}
		return loaded;
	}
	int ModelManager::GetIndex(std::string _name){
		int _index;
		for (int i = 0; i < models.size(); i++) {
			if (_name == models[i].name){
				_index = i;
				break;
			}
		}
		return _index;
	}
	std::vector<int> ModelManager::GetVertCount(int _index){
		return models[_index].vert_count;
	}
	int ModelManager::GetCount(int _index){
		return models[_index].count;
	}
}