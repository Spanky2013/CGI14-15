#pragma once


#include <vector>
#include "glm/glm/glm.hpp"

class BoundingBox
{

public:
	//public vars
	glm::vec3 bBoxMin;
	glm::vec3 bBoxMax;
	glm::vec3 midPoint;
	
	BoundingBox();
	BoundingBox( glm::vec3 boundingBoxMin, glm::vec3 boundingBoxMax);
	~BoundingBox();

	int get_longest_axis();
	bool hit_it(Ray ray, float time);
	static BoundingBox get_bounding_box(std::vector<glm::uvec3> triangles, std::vector<glm::vec3> positions);
};

