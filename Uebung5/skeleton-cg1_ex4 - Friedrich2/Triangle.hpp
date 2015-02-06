#pragma once

#include <vector>
#include "glm/glm/glm.hpp"

class Triangle
{
public:
	Triangle(void);
	
	Triangle(glm::vec3 first, glm::vec3 second, glm::vec3 third);
	~Triangle(void);

	glm::vec3 fir,firN,sec,secN,thi,thiN, midPoint, normal;

	void adNormals(glm::vec3 n1,glm::vec3 n2,glm::vec3 n3 );
};

