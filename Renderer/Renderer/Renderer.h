#pragma once
#include <gl/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "Camera.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "MaterialManager.h"
#include "LightManager.h"
#include "GameObject.h"

#include "ShadowMapFBO.h"
#include "CubeMapFBO.h"

#define SHADOWMAP_SIZE 2048

namespace mor{
	//shadow mapping stuff
	struct CameraDirection{
		GLenum CubemapFace;
		glm::vec3 target;
		glm::vec3 up;

	};

	class Renderer{
	public:
		static Renderer& GetInstance();
		static void ResetInstance();
		//Renderer();
		~Renderer();

		/*Sets the size of the view.
		Must be set before rendering.*/
		inline void SetScreenSize(int _screenWidth, int _screenHeight){
			screen_width = _screenWidth;
			screen_height = _screenHeight;
		}
		inline int ScreenWidth(){ return screen_width; };
		inline int ScreenHeight(){ return screen_height; };

		int LoadModel(std::string _name);
		int LoadShader(std::string vertex_path, std::string fragment_path);
		void BindShader(int _index);
		int LoadTexture(std::string _path);
		int LoadMaterial(glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular, float _shininess);

		//render single object
		void Render(GameObject* _object);
		//render all objects
		void Render(std::vector<GameObject*> _objects);
		void Update(float _delta);

		void SetCamera(Camera *_camera);
		void AddLight(Light *_light);
		void RemoveLight(Light *_light);

		void SetDebug(bool _debug);

		//public for testing
		void ShadowMapPass(std::vector<GameObject*> _objects);
		void CubeMapPass(GameObject* reflect_object, std::vector<GameObject*> _objects);
	private:
		static Renderer *instance;
		Renderer();

		ShaderManager sManager = ShaderManager::GetInstance();
		TextureManager tManager = TextureManager::GetInstance();
		ModelManager mManager = ModelManager::GetInstance();
		MaterialManager matManager = MaterialManager::GetInstance();
		LightManager lManager = LightManager::GetInstance();

		bool debug;

		int screen_width, screen_height;

		void InitVAO();
		void InitUBO();

		void BindObject(GameObject *_object);

		//vertex array object
		GLuint vao;
		//uniform buffer object's
		GLuint camera_ubo, lighting_ubo, material_ubo;
		
		//lighting uniforms
		//GLuint uniCam;

		//for science
		GLuint uniView2, uniProj2;

		//camera object [holds & updates view/projection matrices]
		Camera *camera;


		//more shadow stuff
		ShadowMapFBO *shadowMapFBO;
		CubeMapFBO *cubeMapFBO;

		CameraDirection gCameraDirections[6];
	};

}