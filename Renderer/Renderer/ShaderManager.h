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
	public:
		~ShaderManager();

		static ShaderManager& GetInstance();
		int LoadShader(const char *vertex_path, const char *fragment_path);
		void BindShader(int _index);
		GLuint GetBindedProgram();

		GLuint GetModelUniformL();
		GLuint GetNormalUniformL();
		GLuint GetNumLightsUniform();

		int ShaderCount();
	};

}