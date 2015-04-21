#include "Renderer.h"

namespace mor{
	Renderer* Renderer::instance = nullptr;

	//light data [need to add these light props to a light class]
	glm::vec4 light_position(10.0, 2.0, 10.0, 0.0);
	glm::vec4 light_ambient(0.01, 0.01, 0.01, 1.0);
	glm::vec4 light_diffuse(0.0, 0.0, 0.0, 1.0);
	glm::vec4 light_specular(0.0, 0.0, 0.0, 1.0);
	//[need to add these matrial props to each gameobject]
	glm::vec4 material_ambient(1.0, 1.0, 1.0, 1.0);
	glm::vec4 material_diffuse(0.8, 0.8, 0.8, 1.0);
	glm::vec4 material_specular(0.5, 0.5, 0.5, 1.0);
	GLfloat material_shininess = 25.0f;

	glm::vec4 ambient_product = light_ambient * material_ambient;
	glm::vec4 diffuse_product = light_diffuse * material_diffuse;
	glm::vec4 specular_product = light_specular * material_specular;

	Renderer::Renderer(){
		debug = false;

		InitVAO();
		InitUBO();
		//load a default shader
		LoadShader("default_vert.glsl", "default_frag.glsl");
		//wireframe shader
		LoadShader("red_vert.glsl", "red_frag.glsl");
		LoadShader("shadow_map_vert.glsl", "shadow_map_frag.glsl");
		//bounding sphere load
		LoadModel("bounding_sphere");
		LoadModel("cube");
		//load default material
		matManager.AddMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec4(0.8, 0.8, 0.8, 1.0), glm::vec4(0.5, 0.5, 0.5, 1.0), 25.0f);
		
		shadowMapFBO = new ShadowMapFBO();
		shadowMapFBO->Init(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
		gCameraDirections[0] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) };
		gCameraDirections[1] = { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) };
		gCameraDirections[2] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
		gCameraDirections[3] = { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) };
		gCameraDirections[4] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) };
		gCameraDirections[5] = { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) };

		cubeMapFBO = new CubeMapFBO();
		cubeMapFBO->Init(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	Renderer::~Renderer(){
		glDeleteVertexArrays(1, &vao);
		delete shadowMapFBO;
		delete cubeMapFBO;
	}
	
	Renderer& Renderer::GetInstance(){
		if (instance == nullptr){
			instance = new Renderer();
		}
		return *instance;
	}
	void Renderer::ResetInstance(){
		delete instance;
		instance = nullptr;
	}

	void Renderer::Render(GameObject* _object){
		glBindVertexArray(vao);
		if (_object->IsActive()){
			glm::mat4x4 _modelMatrix = _object->GetModelMatrix();
			_object->bounding_shape->SetCenter(glm::vec3(_modelMatrix[3][0], _modelMatrix[3][1], _modelMatrix[3][2]));
			if (camera->InFrustum(_object->GetModelMatrix())){
				BindObject(_object);
				glUniformMatrix4fv(sManager.GetModelUniformL(), 1, GL_FALSE, glm::value_ptr(_object->GetModelMatrix()));
				glUniformMatrix3fv(sManager.GetNormalUniformL(), 1, GL_FALSE, glm::value_ptr(glm::mat3x3(glm::transpose(glm::inverse(_object->GetModelMatrix())))));

				glDrawElements(GL_TRIANGLES, mManager.GetCount(_object->model), GL_UNSIGNED_INT, 0 * sizeof(GLuint));
			}
			Render(_object->GetChildren());
		}
	}
	void Renderer::Render(std::vector<GameObject*> _objects){
		glBindVertexArray(vao);
		for (int i = 0; i < _objects.size(); i++){
			if (_objects[i]->IsActive()){
				glm::mat4x4 _modelMatrix = _objects[i]->GetModelMatrix();
				_objects[i]->bounding_shape->SetCenter(glm::vec3(_modelMatrix[3][0], _modelMatrix[3][1], _modelMatrix[3][2]));
				if (camera->InFrustum(_objects[i]->bounding_shape)){
					BindObject(_objects[i]);
					glUniformMatrix4fv(sManager.GetModelUniformL(), 1, GL_FALSE, glm::value_ptr(_modelMatrix));
					glUniformMatrix3fv(sManager.GetNormalUniformL(), 1, GL_FALSE, glm::value_ptr(glm::mat3x3(glm::transpose(glm::inverse(_modelMatrix)))));
					glDrawElements(GL_TRIANGLES, mManager.GetCount(_objects[i]->model), GL_UNSIGNED_INT, 0 * sizeof(GLuint));
					
					if (debug){
						//render wireframe
						glm::mat4x4 wire_modelM;
						wire_modelM = glm::translate(wire_modelM, _objects[i]->bounding_shape->Center());
						
						if (Sphere * s = dynamic_cast<Sphere*>(_objects[i]->bounding_shape)) {
							wire_modelM = glm::scale(wire_modelM, glm::vec3(s->Radius() * 2, s->Radius() * 2, s->Radius() * 2));
							mManager.BindModel(0);
						}
						else if (AABox * b = dynamic_cast<AABox*>(_objects[i]->bounding_shape)) {
							wire_modelM = glm::scale(wire_modelM, glm::vec3(b->Width(), b->Height(), b->Length()));
							mManager.BindModel(1);
						}
						sManager.BindShader(1);
						glUniformMatrix4fv(sManager.GetModelUniformL(), 1, GL_FALSE, glm::value_ptr(wire_modelM));
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						glDrawElements(GL_TRIANGLES, mManager.GetCount(0), GL_UNSIGNED_INT, 0 * sizeof(GLuint));
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}
				}
				Render(_objects[i]->GetChildren());
			}
		}
	}
	void Renderer::Update(float _delta){
		if (camera != nullptr){
			sManager.BindShader(0);
			
			camera->Update(_delta);
			
			glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->GetView()));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::vec4), glm::value_ptr(camera->pos));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
		lManager.UpdateUniforms(lighting_ubo);

	}

	void Renderer::SetCamera(Camera *_camera){
		camera = _camera;
		glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->GetView()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->GetProj()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::vec4), glm::value_ptr(camera->pos));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void Renderer::AddLight(Light *_light){
		lManager.AddLight(_light);
		
		for (int i = 0; i < sManager.ShaderCount(); i++){
			sManager.BindShader(i);
			glUniform1i(sManager.GetNumLightsUniform(), lManager.LightCount());
		}
	}
	void Renderer::RemoveLight(Light *_light){
		lManager.RemoveLight(_light);
		
		for (int i = 0; i < sManager.ShaderCount(); i++){
			sManager.BindShader(i);
			glUniform1i(sManager.GetNumLightsUniform(), lManager.LightCount());
		}
	}

	int Renderer::LoadModel(std::string _name){
		return mManager.LoadModel(_name);
	}
	int Renderer::LoadShader(std::string vertex_path, std::string fragment_path){
		int _index = sManager.LoadShader(vertex_path.c_str(), fragment_path.c_str());
		BindShader(_index);
		
		GLuint _uboIndex = glGetUniformBlockIndex(sManager.GetBindedProgram(), "GlobalMatrices");
		glUniformBlockBinding(sManager.GetBindedProgram(), _uboIndex, camera_ubo);
		GLuint _l = glGetUniformBlockIndex(sManager.GetBindedProgram(), "LightingUniforms");
		glUniformBlockBinding(sManager.GetBindedProgram(), _l, lighting_ubo);
		GLuint _m = glGetUniformBlockIndex(sManager.GetBindedProgram(), "MaterialUniforms");
		glUniformBlockBinding(sManager.GetBindedProgram(), _m, material_ubo);
		
		glUniform1i(sManager.GetNumLightsUniform(), lManager.LightCount());

		return _index;
	}
	void Renderer::BindObject(GameObject *_object){
		matManager.BindMaterial(_object->material, material_ubo);
		sManager.BindShader(_object->shader);
		mManager.BindModel(_object->model);
		tManager.BindTexture(_object->texture, GL_TEXTURE0);
		if (_object->normal_map != -1){
			tManager.BindTexture(_object->normal_map, GL_TEXTURE1);
		}
	}
	void Renderer::BindShader(int _index){
		sManager.BindShader(_index);
	}
	int Renderer::LoadTexture(std::string _path){
		return tManager.LoadTexture(_path);
	}
	int Renderer::LoadMaterial(glm::vec4 _ambient, glm::vec4 _diffuse, glm::vec4 _specular, float _shininess){
		return matManager.AddMaterial(_ambient, _diffuse, _specular, _shininess);
	}

	void Renderer::InitVAO(){
		glewInit();
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}
	void Renderer::InitUBO(){
		//camera ubo
		glGenBuffers(1, &camera_ubo);

		glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);

		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2 + sizeof(glm::vec4), NULL, GL_STREAM_DRAW);	
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, camera_ubo, 0, sizeof(glm::mat4) * 2 + sizeof(glm::vec4));

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		//lighting ubo
		glGenBuffers(1, &lighting_ubo);

		glBindBuffer(GL_UNIFORM_BUFFER, lighting_ubo);

		float ubo_size = sizeof(glm::vec4) * 5 + sizeof(float) * 2; //light ubo byte size
		ubo_size *= MAX_LIGHTS;

		glBufferData(GL_UNIFORM_BUFFER, ubo_size, NULL, GL_STREAM_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 2, lighting_ubo, 0, ubo_size);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//material ubo gen
		glGenBuffers(1, &material_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, material_ubo);
		ubo_size = sizeof(glm::vec4) * 3 + sizeof(GLfloat); //material ubo byte size
		glBufferData(GL_UNIFORM_BUFFER, ubo_size, NULL, GL_STREAM_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 3, material_ubo, 0, ubo_size);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	}
	void Renderer::SetDebug(bool _debug){
		debug = _debug;
	}

	//shadow methods
	void Renderer::ShadowMapPass(std::vector<GameObject*> _objects){
		if (lManager.LightCount() > 0){
			if (lManager.IsLightActive(0) && lManager.CastsShadows(0)){ //hard coded for single light only right now
				glBindVertexArray(vao);

				sManager.BindShader(2);
				//save camera state
				glm::vec3 cam_pos = camera->pos;
				glm::vec3 cam_dir = camera->dir;
				glm::vec3 cam_up = camera->up;

				camera->SetPosition(lManager.GetLightPosition(0));
				camera->fov = 90.0f;
				camera->SetScreenSize(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
				glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);

				glCullFace(GL_FRONT);

				for (int i = 0; i < 6; i++){
					shadowMapFBO->BindForWriting(gCameraDirections[i].CubemapFace);

					glClear(GL_DEPTH_BUFFER_BIT);

					camera->dir = gCameraDirections[i].target;
					camera->up = gCameraDirections[i].up;

					SetCamera(camera);
					camera->UpdateFrustum();

					//render each object
					for (int i = 0; i < _objects.size(); i++){
						if (_objects[i]->IsActive()){
							glm::mat4x4 _modelMatrix = _objects[i]->GetModelMatrix();
							//update sphere with parent rot/pos
							_objects[i]->bounding_shape->SetCenter(glm::vec3(_modelMatrix[3][0], _modelMatrix[3][1], _modelMatrix[3][2]));
							if (camera->InFrustum(_objects[i]->bounding_shape)){

								mManager.BindModel(_objects[i]->model);
								glUniformMatrix4fv(sManager.GetModelUniformL(), 1, GL_FALSE, glm::value_ptr(_modelMatrix));
								glDrawElements(GL_TRIANGLES, mManager.GetCount(_objects[i]->model), GL_UNSIGNED_INT, 0 * sizeof(GLuint));

							}
							/*
							//render every child object
							for (int c = 0; c < _objects[i]->GetChildren().size(); c++){
							Render(_objects[i]->GetChildren());
							}
							*/
						}
					}
				}
				//reset everything
				sManager.BindShader(-1);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				camera->SetPosition(cam_pos);
				camera->dir = cam_dir;
				camera->up = cam_up;
				camera->fov = 45.0f;
				camera->SetScreenSize(screen_width, screen_height);
				glViewport(0, 0, screen_width, screen_height);

				SetCamera(camera);
				camera->UpdateFrustum();

				glCullFace(GL_BACK);

				shadowMapFBO->BindForReading(GL_TEXTURE2);

				glActiveTexture(GL_TEXTURE0);
			}
		}
	}
	void Renderer::CubeMapPass(GameObject* reflect_object, std::vector<GameObject*> _objects){
		glBindVertexArray(vao);
		
		//save camera state
		glm::vec3 cam_pos = camera->pos;
		glm::vec3 cam_dir = camera->dir;
		glm::vec3 cam_up = camera->up;

		camera->SetPosition(reflect_object->position);
		camera->fov = 90.0f;
		camera->SetScreenSize(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
		glViewport(0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE);

		for (int i = 0; i < 6; i++){
			cubeMapFBO->BindForWriting(gCameraDirections[i].CubemapFace);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			camera->dir = gCameraDirections[i].target;
			camera->up = gCameraDirections[i].up;

			SetCamera(camera);
			camera->UpdateFrustum();

			//render each object
			for (int i = 0; i < _objects.size(); i++){
				if (_objects[i]->IsActive() && _objects[i] != reflect_object){
					glm::mat4x4 _modelMatrix = _objects[i]->GetModelMatrix();
					//update sphere with parent rot/pos
					_objects[i]->bounding_shape->SetCenter(glm::vec3(_modelMatrix[3][0], _modelMatrix[3][1], _modelMatrix[3][2]));
					if (camera->InFrustum(_objects[i]->bounding_shape)){

						BindObject(_objects[i]);
						glUniformMatrix4fv(sManager.GetModelUniformL(), 1, GL_FALSE, glm::value_ptr(_modelMatrix));
						glUniformMatrix3fv(sManager.GetNormalUniformL(), 1, GL_FALSE, glm::value_ptr(glm::mat3x3(glm::transpose(glm::inverse(_modelMatrix)))));
						glDrawElements(GL_TRIANGLES, mManager.GetCount(_objects[i]->model), GL_UNSIGNED_INT, 0 * sizeof(GLuint));

					}
					/*
					//render every child object
					for (int c = 0; c < _objects[i]->GetChildren().size(); c++){
					Render(_objects[i]->GetChildren());
					}
					*/
				}
			}
		}
		//reset everything
		sManager.BindShader(-1);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		camera->SetPosition(cam_pos);
		camera->dir = cam_dir;
		camera->up = cam_up;
		camera->fov = 45.0f;
		camera->SetScreenSize(screen_width, screen_height);
		glViewport(0, 0, screen_width, screen_height);

		SetCamera(camera);
		camera->UpdateFrustum();

		cubeMapFBO->BindForReading(GL_TEXTURE3);

		glActiveTexture(GL_TEXTURE0);
	}
}