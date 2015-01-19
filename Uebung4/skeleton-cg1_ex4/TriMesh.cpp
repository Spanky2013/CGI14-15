/* ----------------------------------------------------------------
   name:           TriMesh.cpp
   purpose:        cg1_ex3 2014 triangle mesh for OpenGL rendering
   version:	   SKELETON CODE
   TODO:           computeSphereUVs
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include "TriMesh.hpp"
#include <fstream>
#include <string>
#include <iostream>
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

name= fileName;
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

void TriMesh::correctTexture(bool correct){
  textureCorrection= correct;
}

// load triangle mesh in OFF format
void TriMesh::reload(){
  loadOff(name);
}

// load triangle mesh in .OFF format
void TriMesh::loadOff(const string& fileName){
	
	int nodeCount, polyCount, lineCount;
	
	std::ifstream fR;
	fR.open(fileName.c_str());

	string line;
	getline(fR, line);
	if(line == "OFF"){
		getline(fR, line);
		stringstream stream;
		stream << line;
		stream >> nodeCount >> polyCount >> lineCount;

		positions.clear();
		faces.clear();
	
		for(int i=0;i<nodeCount;i++){
			GLfloat x,y,z;
			getline(fR, line);
			stringstream stream;
			stream << line;
			stream >> x >> y >> z;
			positions.push_back(glm::vec3(x,y,z));
			//cout <<"Position "<< x<<" "<< y<<" " << z<<" " <<"Positions size"<<positions.size()<<endl;
		}
		
		for(int i = 0; i < polyCount; i++){
			GLfloat count,inX,inY,inZ;
			getline(fR, line);
			stringstream stream;
			stream << line;
			stream >> count >> inX >> inY >> inZ;

			if(winding == PolygonWinding::CW){
				faces.push_back(glm::uvec3(inX,inY,inZ));	
			}else{
				faces.push_back(glm::uvec3(inX,inZ,inY));	
			}	
			//cout <<"Faces "<< inX <<" "<< inY <<" " << inZ <<" " <<endl<<faces.size()<<endl;
		}	
	}
	fR.close();
	cout << "loadOff done: |V| "<<nodeCount<<" |F| "<<polyCount<<endl;
	cout << "Positions: "<<positions.size()<<" Faces "<<faces.size()<<endl;
	
}


// calculate smooth per-vertex normals
void TriMesh::computeNormals(void){
	normals.clear();
	vector<vec3> faceNormals;
	for(uvec3 face : faces) {
		faceNormals.push_back(
			glm::normalize(
				glm::cross(
					(positions[face.y] - positions[face.x]),
					(positions[face.z] - positions[face.x])
				)
			)
		);	
	}
	for(int i = 0; i < positions.size(); ++i) {
		int numFaces = 0;
		vec3 normal = vec3(0, 0, 0);
		for(int j = 0; j < faces.size(); ++j) {
			if(faces[j].x == i || faces[j].y == i || faces[j].z == i) {
				++numFaces;
				normal += faceNormals[j];
			}
		}
		normal /= numFaces;
		normals.push_back(glm::normalize(normal));
	}
}

  // Compute uv coordinates with a spherical mapping
  // (vertices are projected on a sphere along the normal and classical sphere uv unwrap is used)
void TriMesh::computeSphereUVs(void){}

// draw the mesh using vertex arrays
void TriMesh::draw(void){
	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, &positions[0]);
	glEnableVertexAttribArray(attribVertex);

	glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_FALSE, 0, &normals[0]);
	glEnableVertexAttribArray(attribNormal);

	glDrawElements(GL_TRIANGLES, faces.size()*3, GL_UNSIGNED_INT, &faces[0]);
}
