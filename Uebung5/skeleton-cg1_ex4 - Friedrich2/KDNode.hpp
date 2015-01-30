#pragma once

#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/unsigned_int.hpp"
#include "BoundingBox.hpp"
#include "Ray.hpp"


class KDNode{

public:
	BoundingBox bbox;
	KDNode* left;
	KDNode* right;
	std::vector<glm::uvec3> faces;
	
	KDNode();
	~KDNode();
	
	KDNode* build(std::vector<glm::uvec3> triangles, std::vector<glm::vec3> positions, int depth) const;
	bool KDNode::hit_a_tr(KDNode* node, const Ray ray, float t, float tmin, std::vector<glm::vec3> positions) const;

private:
	glm::vec3 get_midPoint_tr(glm::uvec3 tri, std::vector<glm::vec3> positions) const;
	bool hit_ray_tr(glm::uvec3 triangle, std::vector<glm::vec3> positions, float t, float tmin) const;
	glm::vec3 hitPt_ray_tr(Ray ray, glm::uvec3 triangle, std::vector<glm::vec3> positions) const;
	glm::vec3 KDNode::hitNr_ray_tr(glm::uvec3 triangle, std::vector<glm::vec3> positions) const;
};

