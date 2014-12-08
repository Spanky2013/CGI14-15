/* ----------------------------------------------------------------
   name:           TriMesh.cpp
   purpose:        cg1_ex3 2014 triangle mesh for OpenGL rendering
   version:	   SKELETON CODE
   TODO:           loadOff()
		   draw()
		   computeNormals()
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include "TriMesh.hpp"
#include <sstream>
#include <vector>
#include "math.h"
// use this with care
// might cause name collisions
using namespace glm;

using namespace std;

// NVIDIA wants it like this
// see https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php
const GLuint TriMesh::attribVertex= 0;
const GLuint TriMesh::attribNormal= 2;
const GLuint TriMesh::attribColor= 3;
const GLuint TriMesh::attribTexCoord= 8;

TriMesh::TriMesh(){
  winding= CW;
}

TriMesh::TriMesh(const std::string& filename){

  winding= CW;
  loadOff(filename);
  center();
  unitize();
  computeNormals();
}

TriMesh::~TriMesh(){
}

void TriMesh::setWinding(PolygonWinding winding){
  this->winding= winding;
}

// center model at its origin
void TriMesh::center(void){

  calculateBoundingBox();

  vec3 center= (boundingBoxMin + boundingBoxMax) * vec3(0.5);
  
  for(unsigned int i= 0; i<positions.size(); i++){  
    positions[i]-= center;
  }
  boundingBoxMin-= center;
  boundingBoxMax-= center;
}

// normalize to bounding sphere radius 1
void TriMesh::unitize(void){

  calculateBoundingSphere();

  for(unsigned int i= 0; i<positions.size(); ++i){
    positions[i]/= boundingSphereRadius;
  }
  boundingSphereRadius= 1;
  boundingBoxMin= vec3(-1);
  boundingBoxMax= vec3(1);
}

// calculate bounding sphere
void TriMesh::calculateBoundingSphere(void){

  boundingSphereRadius= 0;
  for(unsigned int i= 0; i<positions.size(); i++){
    vec3 v= positions[i];
    if(length(v) > boundingSphereRadius) boundingSphereRadius= length(v);
  }
}

// calculate bounding box
void TriMesh::calculateBoundingBox(void){

  boundingBoxMin= vec3(numeric_limits<float>::max());
  boundingBoxMax= vec3(numeric_limits<float>::min());
  for(unsigned int i= 0; i<positions.size(); i++){
    if(positions[i].x < boundingBoxMin.x) boundingBoxMin.x= positions[i].x;
    if(positions[i].x > boundingBoxMax.x) boundingBoxMax.x= positions[i].x;
    if(positions[i].y < boundingBoxMin.y) boundingBoxMin.y= positions[i].y;
    if(positions[i].y > boundingBoxMax.y) boundingBoxMax.y= positions[i].y;
    if(positions[i].z < boundingBoxMin.z) boundingBoxMin.z= positions[i].z;
    if(positions[i].z > boundingBoxMax.z) boundingBoxMax.z= positions[i].z;
  }
}

// load triangle mesh in .OFF format
void TriMesh::loadOff(const string& filename){

		string& file = filename + ".off";
	ifstream is(file.c_str(), ios::binary);
	assert(is.is_open());
	string str, contents;
	string delimiter = " ";
	string token = "";
	size_t pos = 0;
	// check whether file is an OFF-file
	getline(is,str);
	//if(!str.compare("OFF")) return;
	// get the number of elements ------------
	// triangles
	getline(is,str);
	stringstream sstr;
	sstr<<str.substr(0,pos = str.find(delimiter));
	sstr>>numTriNodes;
	str.erase(0,pos + delimiter.length());
	// polygons
	stringstream sstr1;
	sstr1<<str.substr(0,pos = str.find(delimiter));
	sstr1>>numPolygones;
	str.erase(0,pos + delimiter.length());
	// edges
	stringstream sstr2;
	sstr2<<str.substr(0,pos = str.find(delimiter));
	sstr2>>numEdges;
	str.erase(0,pos + delimiter.length());
	// --------------------------------------
	int counter = 0;
	while(counter < numTriNodes)
	{
	getline(is,str);
	Vector3f currPoint;
	stringstream sstr3;
	str.substr(0, str.find(delimiter));
	sstr3<<str;
	sstr3>>currPoint.x;
	str.substr(1, str.find(delimiter));
	sstr3<<str;
	sstr3>>currPoint.y;
	str.substr(2, str.find(delimiter));
	sstr3<<str;
	sstr3>>currPoint.z;
	nodePoints->push_back(currPoint);
	counter++;
	}
	counter = 0;
	while(getline(is,str) && counter < numPolygones)
	{
	Polygonf currPoly;
	stringstream sstr4;
	while((pos = str.find(delimiter)) == 0) str.erase(0,pos + delimiter.length());
	str.substr(0, pos = str.find(delimiter));
	sstr4<<str;
	sstr4>>currPoly.n;
	str.erase(0,pos + delimiter.length());
	for(int i=1;i<=currPoly.n;i++)
	{
	int currPoint = 0;
	stringstream sstr5;
	while((pos = str.find(delimiter)) == 0) str.erase(0,pos + delimiter.length());
	sstr5<<str.substr(0,pos);
	sstr5>>currPoint;
	str.erase(0,pos + delimiter.length());
	currPoly.nodes.push_back(currPoint);
	}
	polygones->push_back(currPoly);
	counter++;
	}

}


// calculate smooth per-vertex normals
void TriMesh::computeNormals(void){

}

// draw the mesh using vertex arrays
void TriMesh::draw(void){

}
