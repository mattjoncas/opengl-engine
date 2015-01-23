#include "ShaderManager.h"

namespace mor{

	ShaderManager* ShaderManager::instance = nullptr;

	ShaderManager::ShaderManager(){
		//current = 0;
	}

	ShaderManager::~ShaderManager(){
		//delete shaders
		for (auto it = shaders.begin(); it != shaders.end(); ++it){
			delete *it;
		}
		shaders.clear();
	}

	ShaderManager& ShaderManager::GetInstance(){
		if (instance == nullptr){
			instance = new ShaderManager();
		}
		return *instance;
	}
	void ShaderManager::BindShader(int _index){
		if (_index != current){
			if (_index == -1){
				glUseProgram(0);
				current = -1;
			}
			else if (_index < shaders.size()){
				glUseProgram(shaders[_index]->GetProgram());
				current = _index;
				//std::cout << "Shader " << _index << " binded.\n";
			}
			else{
				std::cout << "Shader index out of range.\n";
			}
		}
	}
	GLuint ShaderManager::GetBindedProgram(){
		return shaders[current]->GetProgram();
	}
	GLuint ShaderManager::GetModelUniformL(){
		return shaders[current]->uniModel;
	}
	GLuint ShaderManager::GetNormalUniformL(){
		return shaders[current]->uniNormal;
	}

	//compile shader [combines vertex & fragment shaders into one GLuint]
	int ShaderManager::LoadShader(const char *vertex_path, const char *fragment_path){
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Read shaders
		std::string vertShaderStr = ReadFile(vertex_path);
		std::string fragShaderStr = ReadFile(fragment_path);
		const char *vertShaderSrc = vertShaderStr.c_str();
		const char *fragShaderSrc = fragShaderStr.c_str();

		GLint result = GL_FALSE;
		int logLength;

		// Compile vertex shader
		std::cout << "Compiling vertex shader.\n";
		glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
		glCompileShader(vertShader);

		// Check vertex shader
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
		std::cout << &vertShaderError[0] << "\n";

		// Compile fragment shader
		std::cout << "Compiling fragment shader.\n";
		glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
		glCompileShader(fragShader);

		// Check fragment shader
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
		std::cout << &fragShaderError[0] << "\n";

		std::cout << "Linking program.\n";
		GLuint program = glCreateProgram();
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);

		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> programError((logLength > 1) ? logLength : 1);
		glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
		std::cout << &programError[0] << "\n";

		//glDetachShader(program, vertShader);
		//glDetachShader(program, fragShader);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		shaders.push_back(new Shader());
		shaders[shaders.size() - 1]->Init(program);

		return shaders.size() - 1;
	}
	std::string ShaderManager::ReadFile(const char *filePath){

		std::fstream shader_file(filePath, std::ios::in);
		std::string shader;

		if (shader_file.is_open()){
			std::stringstream buffer;
			buffer << shader_file.rdbuf();
			shader = buffer.str();
		}
		return shader;
	}

	GLuint ShaderManager::GetNumLightsUniform(){
		return shaders[current]->uniNumLights;
	}

	int ShaderManager::ShaderCount(){
		return shaders.size();
	}
}