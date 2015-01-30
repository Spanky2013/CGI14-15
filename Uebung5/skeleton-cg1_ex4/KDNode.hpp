#include "glm/glm/glm.hpp"
#include "BoundingBox.hpp";
#include <vector>

class KDNode {
	public:
		BoundingBox bbox;
		KDNode* left;
		KDNode* right;
		std::vector<glm::vec3*> triangles;

		KDNode ();

		KDNode* build(std::vector<glm::vec3*>& tris, int depth) const;
};