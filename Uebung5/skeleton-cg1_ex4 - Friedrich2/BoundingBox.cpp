#pragma once

#include <iostream>
#include "BoundingBox.hpp"
#include "glm/glm/glm.hpp"
#include "Ray.hpp"

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

/*
 * Ray-box intersection using IEEE numerical properties to ensure that the
 * test is both robust and efficient, as described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */
bool BoundingBox::hit_it(Ray ray, float time0, float time1){

	glm::vec3 paras[2];
	paras[0] = this->bBoxMin;
	paras[1] = this->bBoxMax;
	
	// Smith's Method
	float txmin,txmax,tymin,tymax,tzmin,tzmax;

	txmin = (paras[ray.sign[0]].x - ray.src.x) * ray.inv_dir.x;
	txmax = (paras[1-ray.sign[0]].x - ray.src.x) * ray.inv_dir.x;
	tymin = (paras[ray.sign[0]].y - ray.src.y) * ray.inv_dir.y;
	tymax = (paras[1-ray.sign[0]].y - ray.src.y) * ray.inv_dir.y;
	if ( (txmin > tymax) || (tymin > txmax) ) 
		return false;

	if (tymin > txmin)
		txmin = tymin;
	if (tymax < txmax)
		txmax = tymax;

	tzmin = (paras[ray.sign[0]].z - ray.src.z) * ray.inv_dir.z;
	tzmax = (paras[1-ray.sign[0]].z - ray.src.z) * ray.inv_dir.z;
	if ( (txmin > tzmax) || (tzmin > txmax) ) 
		return false;

	if (tzmin > txmin)
		txmin = tzmin;
	if (tzmax < txmax)
		txmax = tzmax;

	return ( (txmin < time1) && (txmax > time0) );

}

//Liefert die beiden Schnittpunkte mit der BBox
//wenn time0 > time 1 dann gibt es keinen Schnittpunkt
glm::vec2 BoundingBox::get_times(Ray ray){
	glm::vec3 paras[2];
	paras[0] = this->bBoxMin;
	paras[1] = this->bBoxMax;
	
	// Smith's Method
	float txmin,txmax,tymin,tymax,tzmin,tzmax;

	txmin = (paras[ray.sign[0]].x - ray.src.x) * ray.inv_dir.x;
	txmax = (paras[1-ray.sign[0]].x - ray.src.x) * ray.inv_dir.x;
	tymin = (paras[ray.sign[0]].y - ray.src.y) * ray.inv_dir.y;
	tymax = (paras[1-ray.sign[0]].y - ray.src.y) * ray.inv_dir.y;
	tzmin = (paras[ray.sign[0]].z - ray.src.z) * ray.inv_dir.z;
	tzmax = (paras[1-ray.sign[0]].z - ray.src.z) * ray.inv_dir.z;
	
	glm::vec3 src_to_tmin = glm::vec3(txmin-ray.src.x,tymin-ray.src.y,tzmin-ray.src.z);
	glm::vec3 src_to_tmax = glm::vec3(txmax-ray.src.x,tymax-ray.src.y,tzmax-ray.src.z);

	float time0 = src_to_tmin.x/ray.dir.x;
	if(abs(time0 - src_to_tmin.y/ray.dir.y) > 0.001f || abs(time0 - src_to_tmin.y/ray.dir.y)  > 0.001f )
		std::cout << "it went sth wrong in time0!! "<<time0<<";"<<(src_to_tmin.y/ray.dir.y)<<";"<<(src_to_tmin.z/ray.dir.z);

	float time1 = src_to_tmax.x/ray.dir.x;
	if(abs(time0 - src_to_tmin.y/ray.dir.y) > 0.001f || abs(time0 - src_to_tmin.y/ray.dir.y)  > 0.001f )
		std::cout << "it went sth wrong in time1!! "<<time0<<";"<<(src_to_tmin.y/ray.dir.y)<<";"<<(src_to_tmin.z/ray.dir.z);

	//falls wir in einem Polygon sind kann ein Punkt hinter uns liegen, 
	/*if(time0 < 0)
		time0 = 0;
	if(time1 < 0)
		time1 = 0;
		*/

	return glm::vec2(time0,time1);
}

