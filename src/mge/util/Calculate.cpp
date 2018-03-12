#include "Calculate.hpp"

Calculate::Calculate()
{
}

Calculate::~Calculate()
{
}

glm::vec3 Calculate::Lerp(glm::vec3 pStart, glm::vec3 pEnd, float pPercentage)
{
	return glm::vec3(pStart + pPercentage * (pEnd - pStart)); 
}
