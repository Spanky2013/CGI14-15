#pragma once

#include <string>
#include <iostream>
#include "BoundingBox.hpp"
#include "glm/glm/glm.hpp"
#include "Ray.hpp"
#include "Triangle.hpp"

using namespace std;

BoundingBox::BoundingBox(){
	this->bBoxMax = glm::vec3();
	this->bBoxMin = glm::vec3();
	this->midPoint = glm::vec3();
}

BoundingBox::~BoundingBox(){
}

BoundingBox::BoundingBox( glm::vec3 boundingBoxMin, glm::vec3 boundingBoxMax){
	this->bBoxMax = boundingBoxMax;
	this->bBoxMin = boundingBoxMin;
	this->midPoint = glm::vec3((bBoxMax.x+bBoxMin.x)/2,(bBoxMax.y+bBoxMin.y)/2,(bBoxMax.z+bBoxMin.z)/2);
}

// 0 = x-axis, 1 = y-Axis, 2 = z-axis
int BoundingBox::get_longest_axis() const{
	float x = abs(this->bBoxMin.x-this->bBoxMax.x);
	float y = abs(this->bBoxMin.y-this->bBoxMax.y);
	float z = abs(this->bBoxMin.z-this->bBoxMax.z);

	if(x >= y){
		if(x>=z){
			return 0;
		}else{
			return 2;
		}
	}else{
		if(y >= z){
			return 1;
		}else{
			return 2;
		}
	}

}


BoundingBox BoundingBox::get_bounding_box(std::vector<Triangle> triangles){
  glm::vec3 boundingBoxMin, boundingBoxMax;
  boundingBoxMin = glm::vec3(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
  boundingBoxMax = glm::vec3(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min());
  Triangle t = Triangle();

  for(unsigned int i= 0; i<triangles.size(); i++){
	 
	  glm::vec3 first,second,third;

	  first = triangles[i].fir;
	  if(first.x < boundingBoxMin.x) 
		  boundingBoxMin.x= first.x;
	  if(first.x > boundingBoxMax.x) 
		  boundingBoxMax.x= first.x;
	  if(first.y < boundingBoxMin.y) 
		  boundingBoxMin.y= first.y;
	  if(first.y > boundingBoxMax.y) 
		  boundingBoxMax.y= first.y;
	  if(first.z < boundingBoxMin.z) 
		  boundingBoxMin.z= first.z;
	  if(first.z > boundingBoxMax.z) 
		  boundingBoxMax.z= first.z;

	  second = triangles[i].sec;
	  if(second.x < boundingBoxMin.x) 
		  boundingBoxMin.x= second.x;
	  if(second.x > boundingBoxMax.x) 
		  boundingBoxMax.x= second.x;
	  if(second.y < boundingBoxMin.y) 
		  boundingBoxMin.y= second.y;
	  if(second.y > boundingBoxMax.y) 
		  boundingBoxMax.y= second.y;
	  if(second.z < boundingBoxMin.z) 
		  boundingBoxMin.z= second.z;
	  if(second.z > boundingBoxMax.z) 
		  boundingBoxMax.z= second.z;
	 
	  third = triangles[i].thi;
	  if(third.x < boundingBoxMin.x) 
		  boundingBoxMin.x= third.x;
	  if(third.x > boundingBoxMax.x) 
		  boundingBoxMax.x= third.x;
	  if(third.y < boundingBoxMin.y) 
		  boundingBoxMin.y= third.y;
	  if(third.y > boundingBoxMax.y) 
		  boundingBoxMax.y= third.y;
	  if(third.z < boundingBoxMin.z) 
		  boundingBoxMin.z= third.z;
	  if(third.z > boundingBoxMax.z) 
		  boundingBoxMax.z= third.z;	  
		
	  }
	// So jetzt haben wir die extremen Ecken in dieser Gruppe Dreiecke.


  return BoundingBox(boundingBoxMin, boundingBoxMax);
  
}

/*
 * Ray-box intersection using IEEE numerical properties to ensure that the
 * test is both robust and efficient, as described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */
bool BoundingBox::hit_it(Ray ray, float& time0, float& time1) const{

	if(bBoxMax == bBoxMin)
		return false;

	glm::vec3 paras[2];
	paras[0] = glm::vec3(this->bBoxMin.x,this->bBoxMin.y,this->bBoxMin.z);
	paras[1] = glm::vec3(this->bBoxMax.x,this->bBoxMax.y,this->bBoxMax.z);

	/*paras[0] = this->bBoxMax;
	paras[1] = this->bBoxMin;*/
	// Smith's Method
	float txmin,txmax,tymin,tymax,tzmin,tzmax;

	txmin = (paras[ray.sign[0]].x - ray.src.x) * ray.inv_dir.x;
	txmax = (paras[1-ray.sign[0]].x - ray.src.x) * ray.inv_dir.x;
	tymin = (paras[ray.sign[1]].y - ray.src.y) * ray.inv_dir.y;
	tymax = (paras[1-ray.sign[1]].y - ray.src.y) * ray.inv_dir.y;
	if ( (txmin > tymax) || (tymin > txmax) ) 
		return false;

	if (tymin > txmin)
		txmin = tymin;
	if (tymax < txmax)
		txmax = tymax;

	tzmin = (paras[ray.sign[2]].z - ray.src.z) * ray.inv_dir.z;
	tzmax = (paras[1-ray.sign[2]].z - ray.src.z) * ray.inv_dir.z;
	if ( (txmin > tzmax) || (tzmin > txmax) ) 
		return false;

	if (tzmin > txmin)
		txmin = tzmin;
	if (tzmax < txmax)
		txmax = tzmax;

	bool result = ( (txmin < time1) && (txmax > time0) );

	/*if(result){
		time0 = txmin;
		time1 = txmax;
	}*/

	return result;

}

