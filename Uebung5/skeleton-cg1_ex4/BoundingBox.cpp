#pragma once

#include "BoundingBox.hpp"
#include "glm/glm/glm.hpp"

BoundingBox::BoundingBox(){
}

BoundingBox::~BoundingBox(){
}

BoundingBox::BoundingBox( glm::vec3 boundingBoxMin, glm::vec3 boundingBoxMax){
	this->bBoxMax = boundingBoxMax;
	this->bBoxMin = boundingBoxMin;
	this->midPoint = glm::vec3((bBoxMax.x+bBoxMin.x)/2,(bBoxMax.y+bBoxMin.y)/2,(bBoxMax.z+bBoxMin.z)/2);
}

// 0 = x-axis, 1 = y-Axis, 2 = z-axis
int BoundingBox::get_longest_axis(){
	float x = abs(this->bBoxMax.x - this->bBoxMin.x);
	float y = abs(this->bBoxMax.y - this->bBoxMin.y);
	float z = abs(this->bBoxMax.z - this->bBoxMin.z);

	if(x > y){
		if(x>z){
			return 0;
		}else{
			return 2;
		}
	}else{
		if(y > z){
			return 1;
		}else{
			return 2;
		}
	}

}


BoundingBox BoundingBox::get_bounding_box(std::vector<glm::uvec3> triangles, std::vector<glm::vec3> positions){
  glm::vec3 boundingBoxMin= glm::vec3(std::numeric_limits<float>::max());
  glm::vec3 boundingBoxMax= glm::vec3(std::numeric_limits<float>::min());
  glm::uvec3 t;

  for(unsigned int i= 0; i<triangles.size(); i++){
	  t = triangles[i];
	  glm::vec3 first,second,third;

	  first = positions[t.x];
	  if(first.x < boundingBoxMin.x) boundingBoxMin.x= first.x;
	  if(first.x > boundingBoxMax.x) boundingBoxMax.x= first.x;
	  if(first.y < boundingBoxMin.y) boundingBoxMin.y= first.y;
	  if(first.y > boundingBoxMax.y) boundingBoxMax.y= first.y;
	  if(first.z < boundingBoxMin.z) boundingBoxMin.z= first.z;
	  if(first.z > boundingBoxMax.z) boundingBoxMax.z= first.z;

	  second = positions[t.y];
	  if(second.x < boundingBoxMin.x) boundingBoxMin.x= second.x;
	  if(second.x > boundingBoxMax.x) boundingBoxMax.x= second.x;
	  if(second.y < boundingBoxMin.y) boundingBoxMin.y= second.y;
	  if(second.y > boundingBoxMax.y) boundingBoxMax.y= second.y;
	  if(second.z < boundingBoxMin.z) boundingBoxMin.z= second.z;
	  if(second.z > boundingBoxMax.z) boundingBoxMax.z= second.z;
	 
	  third = positions[t.z];
	  if(third.x < boundingBoxMin.x) boundingBoxMin.x= third.x;
	  if(third.x > boundingBoxMax.x) boundingBoxMax.x= third.x;
	  if(third.y < boundingBoxMin.y) boundingBoxMin.y= third.y;
	  if(third.y > boundingBoxMax.y) boundingBoxMax.y= third.y;
	  if(third.z < boundingBoxMin.z) boundingBoxMin.z= third.z;
	  if(third.z > boundingBoxMax.z) boundingBoxMax.z= third.z;	  
		
	  }
	// So jetzt haben wir die extremen Ecken in dieser Gruppe Dreiecke.


  return BoundingBox(boundingBoxMin, boundingBoxMax);
  
}


