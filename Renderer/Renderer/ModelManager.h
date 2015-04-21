#pragma once
#include "Model.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

namespace mor{

	class ModelManager{
	private:
		static ModelManager *instance;
		std::vector<Model> models;

		ModelManager();
		bool CheckLoaded(std::string _name);
		int GetIndex(std::string _name);

		int current;
	public:
		~ModelManager();

		static ModelManager& GetInstance();
		//load a texture, returns its index in vector
		int LoadModel(std::string _model);

		void BindModel(int _index);

		std::vector<int> GetVertCount(int _index);
		int GetCount(int _index);
	};

}