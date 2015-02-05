#pragma once


#include <vector>
#include "glm/glm/glm.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"

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

	glm::vec2 get_times(Ray ray) const;
	int get_longest_axis() const;
	bool hit_it(Ray ray, float time0, float time1) const;
	static BoundingBox get_bounding_box(std::vector<Triangle> triangles);
};

