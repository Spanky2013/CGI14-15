class BoundingBox {
public:
	static BoundingBox get_bounding_box(const std::vector<glm::vec3*> triangles);

	glm::vec3 BoundingBoxMax;
	glm::vec3 BoundingBoxMin;

};