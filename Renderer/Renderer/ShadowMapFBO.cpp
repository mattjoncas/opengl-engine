#include "ShadowMapFBO.h"


ShadowMapFBO::ShadowMapFBO(){

}

ShadowMapFBO::~ShadowMapFBO(){

}

void ShadowMapFBO::Init(int window_width, int window_height){
	// Create the FBO
	glGenFramebuffers(1, &fbo);
	
	// Create the cube map
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

	for (int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, shadowMap, 0);

	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);

	// Disable reads from the color buffer
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapFBO::BindForWriting(GLenum cube_face){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cube_face, shadowMap, 0);
	
	glDrawBuffer(GL_NONE);
}
void ShadowMapFBO::BindForReading(GLenum TextureUnit){
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
	/*
	GLfloat *data = new GLfloat[1024*1024];
	glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
	printf("data: %f\n", data[512*512]);

	std::ofstream file;
	file.open("data.txt");
	for (int i = 0; i < 1024*1024; i++){
		file << data[i] << ", ";
	}
	file.close();

	delete(data);
	*/
}