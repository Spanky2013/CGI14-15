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
#include "KDNode.hpp"

#include "glm\glm\gtc\constants.hpp"

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
  if(fileName.find("quad.off") == std::string::npos){
	  center();
	  unitize();
  }  
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
	name = fileName;

	int nodeCount, polyCount, lineCount;
	
	std::ifstream fR;
	fR.open(fileName.c_str());

	if(!fR.is_open()) {
		exit(1);
	}

	string line;
	getline(fR, line);
	if(line == "OFF"){
		getline(fR, line);
		stringstream stream;
		stream << line;
		stream >> nodeCount >> polyCount >> lineCount;

		positions.clear();
		faces.clear();
		texCoords.clear();
		normals.clear();
		triangles.clear();
	
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
				triangles.push_back(Triangle(positions[inX],positions[inY],positions[inZ]));
			}else{
				faces.push_back(glm::uvec3(inZ,inY,inX));
				triangles.push_back(Triangle(positions[inX],positions[inY],positions[inZ]));
			}	
			if(i == 0){
				cout<<"Test"<<endl;
				cout <<"Faces "<< inX <<" "<< inY <<" " << inZ <<" " <<endl<<faces.size()<<endl;
				cout <<"Triangle "<< inX << " " << positions[inX].x << " " << positions[inX].y << " " << positions[inX].z<<endl;
			}
			
		}	
	}
	fR.close();
	cout << "loadOff done: |V| "<<nodeCount<<" |F| "<<polyCount<<endl;
	cout << "Positions: "<< positions.size()<<" Faces "<<faces.size()<< " Triangles "<< triangles.size() << endl;
	for(int i = 0; i < triangles.size(); i++){
		cout << "Triangle " << i << ": (" << triangles[i].fir.x << "," << triangles[i].fir.y << "," << triangles[i].fir.z << "); (" 
			<< triangles[i].sec.x << "," << triangles[i].sec.y << "," << triangles[i].sec.z << "); ("
			<< triangles[i].thi.x << "," << triangles[i].thi.y << "," << triangles[i].thi.z << ")"  << endl;
	}
	
}

// load triangle mesh in .OFF format wirh offset
void TriMesh::loadOff(const string& fileName, float off){
	name = fileName;

	int nodeCount, polyCount, lineCount;
	
	std::ifstream fR;
	fR.open(fileName.c_str());

	if(!fR.is_open()) {
		exit(1);
	}

	string line;
	getline(fR, line);
	if(line == "OFF"){
		getline(fR, line);
		stringstream stream;
		stream << line;
		stream >> nodeCount >> polyCount >> lineCount;

		positions.clear();
		faces.clear();
		texCoords.clear();
		normals.clear();
		triangles.clear();
	
		for(int i=0;i<nodeCount;i++){
			GLfloat x,y,z;
			getline(fR, line);
			stringstream stream;
			stream << line;
			stream >> x >> y >> z;
			positions.push_back(glm::vec3(x+off,y,z+off));
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
				triangles.push_back(Triangle(positions[inX],positions[inY],positions[inZ]));
			}else{
				faces.push_back(glm::uvec3(inZ,inY,inX));
				triangles.push_back(Triangle(positions[inX],positions[inY],positions[inZ]));
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
	//normals.clear();
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
void TriMesh::computeSphereUVs(void){	
	
	int id = 0;
	for each(vec3 vec in positions) {
		vec3 d = normals[id];
		vec = normalize(vec);

		float r = glm::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
		


		// (0, 0, 1) -> (0.5, 0.5)
		//float v = 0.5 - acos(-d.y) / glm::pi<float>();
		//float u = atan2(vec.y, -d.z) / (2 * glm::pi<float>());
		
		/*float phi = 0;
		if(vec.x != 0){
			phi = atan(vec.y/vec.x);
			if(phi < 0)
				phi = (2*glm::pi<float>()) - phi;
		}
		phi = phi / (2*glm::pi<float>());
		float theta = acos(vec.z/r);
		if(theta < 0)
				theta = (glm::pi<float>()) - theta;
		theta = theta/(glm::pi<float>());

		float u = phi;
		float v = theta;*/	
		
		//float u = 0.5 + (atan2(vec.x,vec.z))/(2*glm::pi<float>());
		//float v = acos(-vec.y)/(glm::pi<float>());		
		float u = 0.5 + (atan2(vec.x,vec.z))/(2*glm::pi<float>());
		float v = 0.5 + asin(vec.y)/(glm::pi<float>());		

		texCoords.push_back(vec2(u, v));

		++id;
	}}

// draw the mesh using vertex arrays
void TriMesh::draw(void){

	if(!positions.empty()) {
		glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, &positions[0]);
		glEnableVertexAttribArray(attribVertex);
	}
	if(!texCoords.empty()) {
		glVertexAttribPointer(attribTexCoord, 2, GL_FLOAT, GL_FALSE, 0, &texCoords[0]);
		glEnableVertexAttribArray(attribTexCoord);
	}
	if(!normals.empty()) {
		glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_FALSE, 0, &normals[0]);
		glEnableVertexAttribArray(attribNormal);
	}
	if(!faces.empty())
		glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, &faces[0]);

}

void TriMesh::kdTest(){
	KDNode node = KDNode::build(triangles, 0);
	/*if(KDNode::hit_a_tr(&node, Ray(glm::vec3, glm::vec3),)){
		glm::vec3 pt = 
	}else{
		cout<<"no intersection"<< endl;
	}*/
}
