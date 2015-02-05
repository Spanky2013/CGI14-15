#pragma once

#include <vector>
#include "glm/glm/glm.hpp"

class Triangle
{
public:
	Triangle(void);
	
	Triangle(glm::vec3 first, glm::vec3 second, glm::vec3 third);
	~Triangle(void);

	glm::vec3 fir,sec,thi, midPoint, normal;
};

