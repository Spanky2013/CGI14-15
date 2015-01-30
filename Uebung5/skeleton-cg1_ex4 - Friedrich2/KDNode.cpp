#include <vector>
#include "KDNode.hpp"
#include "BoundingBox.hpp"


KDNode::KDNode()
{
}
KDNode::~KDNode()
{
}

KDNode* KDNode::build(std::vector<glm::uvec3> triangles, std::vector<glm::vec3> positions, int depth) const{
	KDNode* node = new KDNode();
	node->faces = triangles;
	node->left = NULL;
	node->right = NULL;
	node->bbox = BoundingBox();

	//Sind wir in einem Blatt ohne Triangle
	if(faces.size() == 0){
		return node;
	}

	node->bbox = BoundingBox::get_bounding_box(triangles, positions);

	//Sind wir in einem Blatt mit genau einem Triangle
	if(faces.size() == 1){		
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->faces = std::vector<glm::uvec3>();
		node->right->faces = std::vector<glm::uvec3>();
		return node;
	}

	glm::vec3 midPoint = node->bbox.midPoint;

	//Jetzt aufteilen

	std::vector<glm::uvec3> leftTris;
	std::vector<glm::uvec3> rightTris;
	int axis = node->bbox.get_longest_axis();

	for(int i = 0; i < triangles.size(); i++){
		glm::vec3 currentMidPoint = get_midPoint_tr(triangles[i], positions);
		switch (axis){
		case 0: // x-Axis	
			if(midPoint.x >= currentMidPoint.x){
				rightTris.push_back(triangles[i]);
			}else{
				leftTris.push_back(triangles[i]);
			}
			break;
		case 1: // y-axis
			if(midPoint.y >= currentMidPoint.y){
				rightTris.push_back(triangles[i]);
			}else{
				leftTris.push_back(triangles[i]);
			}
			break;
		case 2: //z-Axis
			if(midPoint.z >= currentMidPoint.z){
				rightTris.push_back(triangles[i]);
			}else{
				leftTris.push_back(triangles[i]);
			}
			break;
		}
	}

	// Haben die Dreiecke in beide Hälften aufgeteilt.
	// Jetzt kommt noch die Rekursion

	node->left = build(leftTris,positions,depth+1);	
	node->right = build(rightTris,positions,depth+1);

	return node;

}


glm::vec3 KDNode::get_midPoint_tr(glm::uvec3 tri, std::vector<glm::vec3> positions) const{
	glm::vec3 result,first,second,third;

	first = positions[tri.x];
	second = positions[tri.y];
	third = positions[tri.z];

	result.x = (first.x+second.x+third.x)/3;
	result.y = (first.y+second.y+third.y)/3;
	result.z = (first.z+second.z+third.z)/3;

	return result;
}

// time0 ist der Eintrittspunkt in die Box, time1 der Austrittspunkt
bool KDNode::hit_a_tr(KDNode* node, const Ray ray, float time1, float time0, std::vector<glm::vec3> positions) const{
		
		//erstma: hat er überhaupt in die bbox getroffen?

	if(node->bbox.hit_it(ray, time0, time1)){
		bool hit_tr = false;
		glm::vec3 hit_pt, local_hit_pt, normal;

		bool hit_left, hit_right;
		if(node->left->faces.size() > 0 || node->right->faces.size() > 0 ){
			hit_left = hit_a_tr(node->left, ray, time1, time0, positions);
			hit_right = hit_a_tr(node->right, ray,time1,time0, positions);
			return hit_left || hit_right;
		} else {
		// Wir sind in ienem Blatt
			for(int i = 0; i < node->faces.size(); i++){
				if(hit_ray_tr(node->faces[i],positions, time1, time0)){
					hit_tr = true;
					time0 = time1;
					hit_pt = hitPt_ray_tr(ray, node->faces[i],positions);
					normal = hitNr_ray_tr(node->faces[i],positions);
				}
			}
			if(hit_tr){
				
				return true;
			}
		return false;
		}
	}

	return false;
}

glm::vec2 KDNode::get_times(Ray ray){
	if(this->faces.size() > 0){
		return glm::vec2(this->bbox.get_times(ray));
	}
}

bool KDNode::hit_ray_tr(glm::uvec3 triangle, std::vector<glm::vec3> positions, float t, float tmin) const{
	bool result = false;
	//TODO
	return result;
}

glm::vec3 KDNode::hitPt_ray_tr(Ray ray, glm::uvec3 triangle, std::vector<glm::vec3> positions) const{
	glm::vec3 result;
	//TODO
	return result;
}

glm::vec3 KDNode::hitNr_ray_tr(glm::uvec3 triangle, std::vector<glm::vec3> positions) const{
	glm::vec3 result = glm::normalize(
				glm::cross(
					(positions[triangle.y] - positions[triangle.x]),
					(positions[triangle.z] - positions[triangle.x])
				)
			);
	return result;
}