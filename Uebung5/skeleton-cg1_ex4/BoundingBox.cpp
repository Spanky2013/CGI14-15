#include "TriMesh.hpp";

BoundingBox::BoundingBox(glm::vec3 BoundingBoxMax, glm::vec3 BoundingBoxMin){

};


static BoundingBox get_bounding_box(const std::vector<glm::vec3*> triangles){
  boundingBoxMin= vec3(numeric_limits<float>::max());
  boundingBoxMax= vec3(numeric_limits<float>::min());
  glm::vec3 t;

  for(unsigned int i= 0; i<triangles.size(); i++){
	  t = triangles[i];

	  for(unsigned int j=0, j<3; j++){
		if(t[j].x < boundingBoxMin.x) boundingBoxMin.x= t[j].x;
		if(t[j].x > boundingBoxMax.x) boundingBoxMax.x= t[j].x;
		if(t[j].y < boundingBoxMin.y) boundingBoxMin.y= t[j].y;
		if(t[j].y > boundingBoxMax.y) boundingBoxMax.y= t[j].y;
		if(t[j].z < boundingBoxMin.z) boundingBoxMin.z= t[j].z;
		if(t[j].z > boundingBoxMax.z) boundingBoxMax.z= t[j].z;
	  }
  }

  return BoundingBox(boundingBoxMin, boundingBoxMax);

}