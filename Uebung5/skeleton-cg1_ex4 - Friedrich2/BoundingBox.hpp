#pragma once


#include <vector>
#include "glm/glm/glm.hpp"
#include "Ray.hpp"

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

	glm::vec2 get_times(Ray ray);
	int get_longest_axis();
	bool hit_it(Ray ray, float time0, float time1);
	static BoundingBox get_bounding_box(std::vector<glm::uvec3> triangles, std::vector<glm::vec3> positions);
};

