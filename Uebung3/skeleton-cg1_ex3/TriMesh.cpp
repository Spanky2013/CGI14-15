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
#include <iostream>
#include <fstream>
#include <sstream>

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

TriMesh::TriMesh(const std::string& fileName){

  winding= CW;
  loadOff(fileName);
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
void TriMesh::loadOff(const string& fileName){
	positions.clear();
	normals.clear();
	faces.clear();

	ifstream input;
	input.open(fileName.c_str());

	if(!(input.is_open())) { exit(1); };

	string line;
	int lineNr = 0;

	numEdges = 0;

	while(std::getline(input, line,'\n')) {
		//cout <<"lineNr " << lineNr <<endl;
		if(lineNr == 0) {
			if(line != "OFF") { exit(1); };
		}else if(lineNr == 1) {
			stringstream sStream(line);
			sStream >> numVertices >> numPolygons >> numEdges;
			//cout <<"numVertices "<<numVertices<<"numPolygons "<<numPolygons<<"numEdges "<<numEdges<<endl;
		}else{
			if(lineNr < numVertices+2) {
				GLfloat x, y, z;
				stringstream sStream(line);
				sStream >> x >> y >> z;
				positions.push_back(glm::vec3(x,y,z));

				//cout <<"Position "<< x<<" "<< y<<" " << z<<" " <<"Positions size"<<positions.size()<<endl;
	
			}else{
				GLfloat i, a, b, c;
				stringstream sStream(line);
				sStream >> i >> a >> b >> c;

				if(winding == PolygonWinding::CW) {
					faces.push_back(glm::uvec3(a,b,c));
				}else{
					faces.push_back(glm::uvec3(c,b,a));
				}
//				cout <<"Faces "<< a<<" "<< b<<" " << c<<" " <<endl<<faces.size()<<endl;

			}
	
		}
		lineNr++;
	}
	cout << "loadOff done: |V| "<<numVertices<<" |F| "<<numPolygons<<endl;
	cout << "Positions: "<<positions.size()<<" Faces "<<faces.size()<<endl;
}


// calculate smooth per-vertex normals
void TriMesh::computeNormals(void){
	
}

// draw the mesh using vertex arrays
void TriMesh::draw(void){
	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, &positions[0]);
	glEnableVertexAttribArray(attribVertex);

	glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_FALSE, 0, &normals[0]);
	glEnableVertexAttribArray(attribNormal);

	glDrawElements(GL_TRIANGLES, faces.size()*3, GL_UNSIGNED_INT, &faces[0]);
}
