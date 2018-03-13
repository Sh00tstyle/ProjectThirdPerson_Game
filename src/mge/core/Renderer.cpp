#include "Renderer.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "World.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"
#include"mge/MGEDemo.hpp"
#include <iostream>

Renderer::Renderer(int windowWidth, int windowHeight):debug(false)
{
    //make sure we test the depthbuffer
	glEnable(GL_DEPTH_TEST);

	//tell opengl which vertex winding is considered to be front facing
	glFrontFace(GL_CCW);

	//tell opengl to enable face culling for the back faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//set the default blend mode aka dark magic:
	//https://www.opengl.org/sdk/docs/man/html/glBlendFunc.xhtml
    //https://www.opengl.org/wiki/Blending
    //http://www.informit.com/articles/article.aspx?p=1616796&seqNum=5
    //http://www.andersriggelsen.dk/glblendfunc.php
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor((float)0x2d / 0xff, (float)0x6b / 0xff, (float)0xce / 0xff, 1.0f);

	//setup framebuffer, renderbuffer and texture for bloom
	glGenFramebuffers(1, &_framebufferId); //generate framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);

	//set two draw buffers
	GLenum fboBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, fboBuffers);

	//main buffer texture
	glGenTextures(1, &_mainbufferId); //generate texture
	glBindTexture(GL_TEXTURE_2D, _mainbufferId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, 0); //generate empty texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _mainbufferId, 0); //attach image to framebuffer

	//glow buffer texture
	glGenTextures(1, &_glowbufferId); //generate texture
	glBindTexture(GL_TEXTURE_2D, _glowbufferId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, 0); //generate empty texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach image to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _glowbufferId, 0);

	//generate renderbuffer
	glGenRenderbuffers(1, &_renderbufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderbufferId);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer initialized" << std::endl;
		MGEDemo::UpdateLoadingScreen(2);
	}

	//blur fbo and textures
	glGenFramebuffers(2, _pingpongFBO);
	glGenTextures(2, _pingpongBuffers);
	for(unsigned int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, _pingpongBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pingpongBuffers[i], 0
		);
	}

	//depth buffer and texture
	glGenFramebuffers(1, &_depthbufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthbufferId);

	glGenTextures(1, &_depthTexId);
	glBindTexture(GL_TEXTURE_2D, _depthTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthTexId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Depthbuffer initialized" << std::endl;
		MGEDemo::UpdateLoadingScreen(5);
	}

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind framebuffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind renderbuffer

	//blur shader
	_blurShader = new ShaderProgram();
	_blurShader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "blurshader.vs");
	MGEDemo::UpdateLoadingScreen(7);
	_blurShader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "blurshader.fs");
	MGEDemo::UpdateLoadingScreen(10);
	_blurShader->finalize();

	_aVertexBlur = _blurShader->getAttribLocation("vertex");
	_aUVBlur = _blurShader->getAttribLocation("uv");
	_uScreenTextureBlur = _blurShader->getUniformLocation("screenTexture");
	_uHorizontalBlur = _blurShader->getUniformLocation("horizontal");

	//screen shader
	_screenShader = new ShaderProgram();
	_screenShader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "screenshader.vs");
	MGEDemo::UpdateLoadingScreen(12);
	_screenShader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "screenshader.fs");
	MGEDemo::UpdateLoadingScreen(14);
	_screenShader->finalize();

	_aVertexScreen = _screenShader->getAttribLocation("vertex");
	_aUVScreen = _screenShader->getAttribLocation("uv");
	_uScreenTextureHDR = _screenShader->getUniformLocation("screenTexture");
	_uScreenTextureBloom = _screenShader->getUniformLocation("bloomTexture");
	_uExposureScreen = _screenShader->getUniformLocation("exposure");

	//screen space quad
	_screenQuad = Mesh::load(config::MGE_MODEL_PATH + "screen_plane.obj");
	MGEDemo::UpdateLoadingScreen(17);
}

Renderer::~Renderer()
{
	//cleanup
	delete _screenQuad;
	glDeleteFramebuffers(1, &_framebufferId);
	glDeleteFramebuffers(2, _pingpongFBO);
	//glDeleteRenderbuffers(1, &_renderbufferId);

	delete _blurShader;
	delete _screenShader;
}

void Renderer::setClearColor(GLbyte pR, GLbyte pG, GLbyte pB) {
	glClearColor((float)pR / 0xff, (float)pG / 0xff, (float)pB / 0xff, 1.0f);
}

void Renderer::setClearColor(float red, float green, float blue, float alpha) {
	glClearColor(red, green, blue, alpha);
}

void Renderer::setClearColor(int red, int green, int blue, int alpha) {
	glClearColor(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}

void Renderer::render(World* pWorld) {
	render(pWorld, pWorld, nullptr, pWorld->getMainCamera(), true);
}

void Renderer::render(World* pWorld, GameObject* pGameObject, AbstractMaterial* pMaterial, Camera* pCamera, bool pRecursive)
{
	render(pWorld, pGameObject, pMaterial, pGameObject->getWorldTransform(), glm::inverse(pCamera->getWorldTransform()), pCamera->getProjection(), pRecursive);
}

void Renderer::render(World* pWorld, GameObject* pGameObject, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive) {
	renderSelf(pWorld, pGameObject, pMaterial == nullptr?pGameObject->getMaterial():pMaterial, pModelMatrix, pViewMatrix, pProjectionMatrix);
	if (pRecursive) renderChildren(pWorld, pGameObject, pMaterial, pModelMatrix, pViewMatrix, pProjectionMatrix, pRecursive);
}

void Renderer::renderSelf(World* pWorld, GameObject* pGameObject, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	render(pWorld, pGameObject->getMesh(), pMaterial, pModelMatrix, pViewMatrix, pProjectionMatrix);
	if (debug) renderMeshDebugInfo(pGameObject->getMesh(), pModelMatrix, pViewMatrix, pProjectionMatrix);
}

void Renderer::renderChildren(World* pWorld, GameObject* pGameObject, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive) {
	int childCount = pGameObject->getChildCount();
	if (childCount < 1) return;

	//note that with a loop like this, deleting children during rendering is not a good idea :)
	GameObject* child = 0;
	for (int i = 0; i < childCount; i++) {
		child = pGameObject->getChildAt(i);
		render(pWorld, child, pMaterial, pModelMatrix * child->getTransform(), pViewMatrix, pProjectionMatrix, pRecursive);
	}
}

void Renderer::render(World* pWorld, Mesh* pMesh, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if(pMesh != nullptr && pMaterial != nullptr) {
		if(_depthOnly) pMaterial->renderDepth(pWorld, pMesh, pModelMatrix, pViewMatrix, pProjectionMatrix);
		else pMaterial->render(pWorld, pMesh, pModelMatrix, pViewMatrix, pProjectionMatrix);
	}
}

void Renderer::renderMeshDebugInfo(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (pMesh != nullptr) pMesh->drawDebugInfo(pModelMatrix, pViewMatrix, pProjectionMatrix);
}

void Renderer::useFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	_depthOnly = false;
}

void Renderer::unbindFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawFramebuffer(bool usePP) {
	//draws the scene to the screen (with post processing)
	glDisable(GL_DEPTH_TEST);

	if(usePP) {
		//render to framebuffers and blur
		bool horizontal = true, first_iteration = true;
		int amount = 10;

		_blurShader->use(); //blur
		for(unsigned int i = 0; i < amount; i++) {
			glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[horizontal]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? _glowbufferId : _pingpongBuffers[!horizontal]);
			//pass in texture and horizontal uniforms to the shader
			glUniform1i(_uScreenTextureBlur, 0);
			glUniform1i(_uHorizontalBlur, horizontal);

			//render quad to framebuffer
			_screenQuad->streamToOpenGL(_aVertexBlur, -1, _aUVBlur);

			horizontal = !horizontal;
			if(first_iteration)
				first_iteration = false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//render to screen
		_screenShader->use(); //bloom

		//tell the shader that we are using pp
		glUniform1i(_screenShader->getUniformLocation("usePP"), true);

		//pass in uniforms to the shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _mainbufferId); //screen texture
		glUniform1i(_uScreenTextureHDR, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _pingpongBuffers[0]); //blurred texture
		glUniform1i(_uScreenTextureBloom, 1);

		glUniform1f(_uExposureScreen, 1.0f);

		//render quad to screen
		_screenQuad->streamToOpenGL(_aVertexScreen, -1, _aUVScreen);
	} else {
		//just render the scene to the quad
		_screenShader->use();

		//tell the shader that we are not using pp
		glUniform1i(_screenShader->getUniformLocation("usePP"), false);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _depthTexId); //screen texture
		glUniform1i(_uScreenTextureHDR, 0);

		//render quad to screen
		_screenQuad->streamToOpenGL(_aVertexScreen, -1, _aUVScreen);
	}
}

void Renderer::useDepthbuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, _depthbufferId);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	_depthOnly = true;
}

void Renderer::unbindDepthbuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::setScreenSizes(int windowSizeX, int windowSizeY) {
	//main buffer
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferId);
	glBindTexture(GL_TEXTURE_2D, _mainbufferId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowSizeX, windowSizeY, 0, GL_RGB, GL_FLOAT, 0); //generate empty texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _mainbufferId, 0);

	 //glowbuffer
	glBindTexture(GL_TEXTURE_2D, _glowbufferId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowSizeX, windowSizeY, 0, GL_RGB, GL_FLOAT, 0); //generate empty texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _glowbufferId, 0);

	//blurbuffers
	glGenTextures(2, _pingpongBuffers);
	for(unsigned int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, _pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, _pingpongBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, windowSizeX, windowSizeY, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _pingpongBuffers[i], 0
		);
	}

	//renderbuffer
	glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowSizeX, windowSizeY);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderbufferId);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer size update" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind framebuffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind renderbuffer
}