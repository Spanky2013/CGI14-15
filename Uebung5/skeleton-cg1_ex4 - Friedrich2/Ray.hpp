#pragma once

#include "glm/glm/glm.hpp"

class Ray
{
public:
	Ray(void);
	~Ray(void);

	Ray(glm::vec3 source, glm::vec3 direction);

	glm::vec3 src;
	glm::vec3 dir;

	glm::vec3 posAt(float time) const;
};

