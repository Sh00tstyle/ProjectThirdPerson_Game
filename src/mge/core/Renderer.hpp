#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>
#include "glm.hpp"
#include <float.h>

class World;
class GameObject;
class Mesh;
class AbstractMaterial;
class Camera;
class ShaderProgram;

/**
* Renderer implements a default single pass forward renderer.
*/
class Renderer
{
public:
	Renderer(int windowWidth, int windowHeight);
	virtual ~Renderer();

	bool debug;

	/**
	 * Utility call
	 */
	void setClearColor(GLbyte pR, GLbyte pG, GLbyte pB);
	void setClearColor(float red, float green, float blue, float alpha); //overloading for floats
	void setClearColor(int red, int green, int blue, int alpha); //overloading for ints

	/**
	 * Convenience method to render whole world, visits each gameobject in the world recursively,
	 * using each gameobject's mesh & material, and the world's main camera.
	 */
	void render(World* pWorld);

	/**
	 * Renders a specific GameObject in the world using a specific camera.
	 * Convenience method for calling render (world, gameobject, material, modelmatrix, viewmatrix, projectionmatrix, recursive)
	 *
	 * @param pWorld the world the mesh is a part of, can be used to retrieve lighting information
	 * @param pGameObject the gameobject to render, will use the this gameobject's world transform for the model matrix
	 * @param pMaterial the material to use, is passed on to render (world, gameobject, material, modelmatrix, viewmatrix, projectionmatrix, recursive)
	 * @param pCamera used for the view and perspective matrix
     * @param pRecursive render everything recursively
	 */
	void render(World* pWorld, GameObject* pGameObject, AbstractMaterial* pMaterial, Camera* pCamera, bool pRecursive);

	/**
	 * Renders a specific game object in the world by calling:
	 *  renderSelf
	 *  renderChildren
	 *
	 * @param pWorld the world the gameobject is a part of, can be used to retrieve lighting information
	 * @param pGameObject the gameobject to render
	 * @param pMaterial the material to use, if NULL, the pGameObject->getMaterial() will be used, if NOT null it overrides the material for each and every gameobject
	 * @param pModelMatrix the world transform of the model
	 * @param pViewMatrix the view matrix of the camera
	 * @param pProjectionMatrix the projectionmatrix of the camera
	 * @param pRecursive render everything recursively
	 */
	void render(World* pWorld, GameObject* pGameObject, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive);

	/**
	* Renders a specific mesh in the world.
	*
	* @param pWorld the world the mesh is a part of, can be used to retrieve lighting information
	* @param pMesh the mesh to render
	* @param pMaterial the material to use, CANNOT be NULL !!
	* @param pModelMatrix the world transform of the model
	* @param pViewMatrix the view matrix of the camera
	* @param pProjectionMatrix the projectionmatrix of the camera
	*/
	void render(World* pWorld, Mesh* pMesh, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);

	/**
	 * Helper method to render debug info for a mesh
	 */
	void renderMeshDebugInfo(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);

	void useFramebuffer();
	void unbindFramebuffer();
	void drawFramebuffer(bool usePP);

	void setScreenSizes(int windowSizeX, int windowSizeY);

	void useDepthbuffer();
	void unbindDepthbuffer();

protected:

	/**
	* Renders a specific game object in the world.
	*
	* @param pWorld the world the gameobject is a part of, can be used to retrieve lighting information
	* @param pGameObject the gameobject to render
	* @param pMaterial the material to use, CANNOT BE NULL
	* @param pModelMatrix the world transform of the model
	* @param pViewMatrix the view matrix of the camera
	* @param pProjectionMatrix the projectionmatrix of the camera
	*/
	void renderSelf(World* pWorld, GameObject* pGameObject, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);

	//calls render on each and every child gameobject, material cannot be null!
	void renderChildren(World* pWorld, GameObject* pGameObject, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive);

private:
	bool _depthOnly;

	//framebuffers and renderbuffers
	GLuint _framebufferId;
	GLuint _depthbufferId;
	GLuint _pingpongFBO[2];
	GLuint _renderbufferId;

	//textures
	GLuint _mainbufferId;
	GLuint _glowbufferId;
	GLuint _depthTexId;
	GLuint _pingpongBuffers[2];

	//screen shaders
	ShaderProgram* _blurShader;
	ShaderProgram* _screenShader;

	//attributes and uniforms
	GLint _aVertexBlur;
	GLint _aUVBlur;
	GLint _uScreenTextureBlur;
	GLint _uHorizontalBlur;

	GLint _aVertexScreen;
	GLint _aUVScreen;
	GLint _uScreenTextureHDR;
	GLint _uScreenTextureBloom;
	GLint _uExposureScreen;

	Mesh* _screenQuad;
};

#endif // RENDERER_HPP

