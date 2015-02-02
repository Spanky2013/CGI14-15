#pragma once

#include "glm/glm/glm.hpp"

class RayTraceHelper
{
public:
	RayTraceHelper(void);
	~RayTraceHelper(void);
	
	glm::vec3 intersectionPoint;
	glm::vec3 normalAtIntSec;
};

