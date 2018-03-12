#include "glm.hpp"


#pragma once
 class Calculate {
 public:
	 Calculate(); 
	 ~Calculate(); 

	 static glm::vec3 Lerp(glm::vec3 pStart, glm::vec3 pEnd, float pPercentage);


};

