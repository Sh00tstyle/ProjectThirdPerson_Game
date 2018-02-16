#include "mge/behaviours/PlayfieldFocusBehaviour.h"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Mouse.hpp>
#include "mge/core/Camera.hpp"
#include <iostream>

PlayfieldFocusBehaviour::PlayfieldFocusBehaviour(float pXAngle, float pYAngle, float pYOffset, float pZOffset, Camera* pCamera):AbstractBehaviour(), _xAngle(pXAngle), _yAngle(pYAngle), _yOffset(pYOffset), _zOffset(pZOffset) {
	//constructor
	ApplyRotation(pCamera);
}

PlayfieldFocusBehaviour::~PlayfieldFocusBehaviour() {
	//destructor
}

void PlayfieldFocusBehaviour::update(float step) {

}

void PlayfieldFocusBehaviour::ApplyRotation(Camera* camera) {
	//first rotate the camera around itself on the x axis
	glm::mat4 cameraMatrix = glm::rotate(glm::radians(_xAngle), glm::vec3(1.0f, 0.0f, 0.0f));

	//translate the camera to the target offset position
	cameraMatrix = glm::translate(glm::vec3(0, _yOffset, _zOffset)) * cameraMatrix;

	//rotate the camera around the y world axis
	cameraMatrix = glm::rotate(glm::radians(_yAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * cameraMatrix;

	camera->setTransform(cameraMatrix);
}