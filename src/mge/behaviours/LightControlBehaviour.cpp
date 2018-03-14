#include "mge/behaviours/LightControlBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Light.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/config.hpp"
#include <SFML/Window/Keyboard.hpp>

LightControlBehaviour::LightControlBehaviour(Light* pLight, float pMoveSpeed) : AbstractBehaviour(), _light(pLight), _moveSpeed(pMoveSpeed), _startTransform(pLight->getTransform()) {
	//constructor
	
	_debugPlane = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	pLight->setMesh(_debugPlane);
}

LightControlBehaviour::~LightControlBehaviour() {
	//destructor

	delete _debugPlane;
}

void LightControlBehaviour::update(float pStep) {
	//rotating around (for directional light)
	/**
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _owner->rotate(pStep, glm::vec3(0.0f, 1.0f, 0.0f));
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _owner->rotate(-pStep, glm::vec3(0.0f, 1.0f, 0.0f));
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) _owner->rotate(-pStep, glm::vec3(1.0f, 0.0f, 0.0f));
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) _owner->rotate(pStep, glm::vec3(1.0f, 0.0f, 0.0f));
	/**/

	glm::vec3 position = _owner->getLocalPosition();
	glm::mat4 newTransform = glm::translate(glm::vec3(0, 5, 0));

	float speed = 20;

	/**/
	//simple movement (for point light)
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		//_owner->translate(glm::vec3(0.0f, 0.0f, _moveSpeed * pStep));

		_rotaXMat = glm::rotate(_rotaXMat, glm::radians(speed * pStep), glm::vec3(1, 0, 0));
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		//_owner->translate(glm::vec3(0.0f, 0.0f, -_moveSpeed * pStep));

		_rotaXMat = glm::rotate(_rotaXMat, -glm::radians(speed * pStep), glm::vec3(1, 0, 0));
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		//_owner->translate(glm::vec3(_moveSpeed*pStep, 0.0f, 0.0f));

		_rotaZMat = glm::rotate(_rotaZMat, glm::radians(speed * pStep), glm::vec3(0, 0, 1));
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		//_owner->translate(glm::vec3(-_moveSpeed * pStep, 0.0f, 0.0f));

		_rotaZMat = glm::rotate(_rotaZMat, -glm::radians(speed * pStep), glm::vec3(0, 0, 1));
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		//_owner->translate(glm::vec3(0.0f, _moveSpeed*pStep, 0.0f));

		_rotaYMat = glm::rotate(_rotaYMat, glm::radians(speed * pStep), glm::vec3(0, 1, 0));
	} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		//_owner->translate(glm::vec3(0.0f, -_moveSpeed * pStep, 0.0f));

		_rotaYMat = glm::rotate(_rotaYMat, -glm::radians(speed * pStep), glm::vec3(0, 1, 0));
	}
	/**/

	newTransform = _rotaXMat * _rotaYMat * _rotaZMat * newTransform;

	_owner->setTransform(newTransform);
	TextureMaterial::SetLightPos(_owner->getLocalPosition()); //apply position to light

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		glm::vec3 lightPos = _owner->getLocalPosition();

		std::cout << "Light at: x = " + std::to_string(lightPos.x) + ",y = " + std::to_string(lightPos.y) + ",z = " + std::to_string(lightPos.z)<< std::endl;
	}

	//reset light position and rotation
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		_owner->setTransform(glm::translate(glm::vec3(0, 5, 0)));
		_rotaXMat = glm::mat4();
		_rotaYMat = glm::mat4();
		_rotaZMat = glm::mat4();
	}

	/**
	//switch light type
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) _light->setLightType(LightType::DIRECTIONAL);
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) _light->setLightType(LightType::POINT);
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) _light->setLightType(LightType::SPOT);
	/**/
}