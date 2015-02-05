#pragma once

#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/unsigned_int.hpp"
#include "BoundingBox.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"
#include "RayTraceHelper.hpp"

class KDNode{

public:
	BoundingBox bbox;
	KDNode* left;
	KDNode* right;
	std::vector<Triangle> triangles;
	
	KDNode();
	~KDNode();
	
	static KDNode* build(std::vector<Triangle> triangles, int depth);
	/*glm::vec2 KDNode::get_times(Ray ray);*/
	bool KDNode::hit_a_tr(KDNode* node, const Ray ray, float t, float tmin, RayTraceHelper& rtHelper);

private:

	glm::vec3 hitPoint;

	bool hit_ray_tr(const Ray ray, Triangle triangle, float t, float tmin);
	glm::vec3 hitPt_ray_tr(Ray ray, Triangle tri) const;
	glm::vec3 KDNode::hitNr_ray_tr(glm::vec3 dir, Triangle tri) const;
};

