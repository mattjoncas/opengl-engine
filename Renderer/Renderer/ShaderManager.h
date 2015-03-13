#pragma 
#include <iostream>
#include <fstream>
#include <vector>
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>
#include "Shader.h"

namespace mor{

	class ShaderManager{
	private:
		static ShaderManager *instance;
		std::vector<Shader*> shaders;

		ShaderManager();
		std::string ReadFile(const char *filePath);
		int current;

		bool IsLoaded(std::string _name);
	public:
		~ShaderManager();

		static ShaderManager& GetInstance();
		//loads a glsl shader from within the 'shaders' folder
		int LoadShader(const char *vertex_name, const char *fragment_name);
		void BindShader(int _index);
		GLuint GetBindedProgram();

		GLuint GetModelUniformL();
		GLuint GetNormalUniformL();
		GLuint GetNumLightsUniform();

		int ShaderCount();
		int GetShaderIndex(std::string _name);
	};

}