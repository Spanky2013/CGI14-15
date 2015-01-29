#include "KDNode.h"


KDNode::KDNode(void)
{
}


KDNode* KDNode::build(std::vector<glm::uvec3*>& triangles, int depth) const{
	KDNode* node = new KDNode();
	node->faces = triangles;
	node->left = NULL;
	node->right = NULL;
	node->bbox = BoundingBox();

	//Sind wir in einem Blatt ohne Triangle
	if(faces.size() == 0){
		return node;
	}

	//Sind wir in einem Blatt mit genau einem Triangle
	if(faces.size() == 1){
		node->bbox = triangles[0]->get_bounding_box();
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->faces = std::vector<glm::uvec3*>();
		node->right->faces = std::vector<glm::uvec3*>();
		return node;
	}


}