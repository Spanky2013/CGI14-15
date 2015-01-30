#include "KDNode.hpp";

using namespace std;

KDNode* KDNode::build(vector<glm::vec3*>& tris, int depth) const{
	KDNode* node = new KDNode();
	node->triangles = tris;
	node->left = NULL;
	node->right = NULL;
	node->bbox = BoundingBox();

	if(tris.size() == 0)
		return node;

	if(tris.size() == 1) {
		node->bbox = BoundingBox::get_bounding_box(node->triangles);
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->triangles = vector<glm::vec3*>();
		node->right->triangles = vector<glm::vec3*>();
		return node;
	}

	node->bbox = BoundingBox::get_bounding_box(node->triangles);
};