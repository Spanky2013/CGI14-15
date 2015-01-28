#pragma once

#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/unsigned_int.hpp"
#include "BoundingBox.h"


class KDNode{

public:
	BoundingBox bbox;
	KDNode* left;
	KDNode* right;

	std::vector<glm::uvec3*> faces;


	KDNode();

	KDNode* build(std::vector<glm::uvec3*>& triangles, int depth) const;
	
};

