#include "mge/level/Particle.h"

Particle::Particle(float pXPos, float pYPos, float pZPos, int pCol, int pRow, float pSpeed, AbstractMaterial* pParticleMat) {
	_xPos = pXPos;
	_yPos = pYPos;
	_zPos = pZPos;

	_col = pCol;
	_row = pRow;
	_speed = pSpeed;

	_particleMat = pParticleMat;
}

Particle::~Particle() {
	delete _particleMat;
}

glm::vec3 Particle::GetParticlePos() {
	return glm::vec3(_xPos, _yPos, _zPos);
}

int Particle::GetCol() {
	return _col;
}

int Particle::GetRow() {
	return _row;
}

float Particle::GetAnimSpeed() {
	return _speed;
}

AbstractMaterial * Particle::GetParticleMat() {
	return _particleMat;
}

void Particle::SetParticleMat(AbstractMaterial * particleMat) {
	_particleMat = particleMat;
}
