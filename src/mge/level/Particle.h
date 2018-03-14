#ifndef PARTICLE_H
#define PARTICLE_H

#include "glm.hpp"
#include "mge/materials/AbstractMaterial.hpp"

class Particle
{
	public:
		Particle(float pXPos, float pYPos, float pZPos, int pCol, int pRow, float pSpeed, AbstractMaterial* pParticleMat);
		~Particle();

		glm::vec3 GetParticlePos();
		int GetCol();
		int GetRow();
		float GetAnimSpeed();

		AbstractMaterial* GetParticleMat();

		void SetParticleMat(AbstractMaterial* particleMat);

	private:
		//pos
		float _xPos;
		float _yPos;
		float _zPos;

		//spritesheet props
		int _col;
		int _row;
		float _speed;

		AbstractMaterial* _particleMat;

};

#endif