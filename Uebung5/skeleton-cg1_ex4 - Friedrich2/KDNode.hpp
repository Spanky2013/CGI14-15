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
	glm::vec2 KDNode::get_times(Ray ray);
	bool KDNode::hit_a_tr(KDNode* node, const Ray ray, float t, float tmin, std::vector<glm::vec3> positions);

private:

	glm::vec3 hitPoint;

	glm::vec3 get_midPoint_tr(glm::uvec3 tri, std::vector<glm::vec3> positions) const;
	bool hit_ray_tr(const Ray ray, glm::uvec3 triangle, std::vector<glm::vec3> positions, float t, float tmin);
	glm::vec3 hitPt_ray_tr(Ray ray, glm::uvec3 triangle, std::vector<glm::vec3> positions) const;
	glm::vec3 KDNode::hitNr_ray_tr(glm::uvec3 triangle, std::vector<glm::vec3> positions) const;
};

