#pragma once

#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/unsigned_int.hpp"
#include "BoundingBox.hpp"


class KDNode{

public:
	BoundingBox bbox;
	KDNode* left;
	KDNode* right;
	std::vector<glm::uvec3> faces;
	
	KDNode();
	~KDNode();
	
	KDNode* build(std::vector<glm::uvec3> triangles, std::vector<glm::vec3> positions, int depth) const;

private:
	glm::vec3 get_midPoint_tr(glm::uvec3 tri, std::vector<glm::vec3> positions) const;
	
};

