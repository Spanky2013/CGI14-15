#pragma once

#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/unsigned_int.hpp"
#include "BoundingBox.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"


class KDNode{

public:
	BoundingBox bbox;
	KDNode* left;
	KDNode* right;
	std::vector<Triangle> triangles;
	
	KDNode();
	~KDNode();
	
	KDNode* build(std::vector<Triangle> triangles, int depth) const;
	glm::vec2 KDNode::get_times(Ray ray);
	bool KDNode::hit_a_tr(KDNode* node, const Ray ray, float t, float tmin, std::vector<glm::vec3> positions);

private:

	glm::vec3 hitPoint;

	glm::vec3 get_midPoint_tr(Triangle tri) const;
	bool hit_ray_tr(const Ray ray, Triangle triangle, float t, float tmin);
	glm::vec3 hitPt_ray_tr(Ray ray, Triangle tri) const;
	glm::vec3 KDNode::hitNr_ray_tr(Triangle tri) const;
};

