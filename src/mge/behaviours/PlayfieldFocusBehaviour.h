#include "mge/behaviours/AbstractBehaviour.hpp"
#include <SFML/System/Vector2.hpp>
#include "glm.hpp"

class Camera;

class PlayfieldFocusBehaviour : public AbstractBehaviour {
	public:
	PlayfieldFocusBehaviour(float pXAngle, float pYAngle, float pYOffset, float pZOffset, Camera* pCamera);
	virtual ~PlayfieldFocusBehaviour();

	virtual void update(float pStep);

	private:
	glm::vec3 _startingPos;

	float _yOffset;
	float _zOffset;
	float _xAngle;
	float _yAngle;

	void ApplyRotation(Camera* camera);
};