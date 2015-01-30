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