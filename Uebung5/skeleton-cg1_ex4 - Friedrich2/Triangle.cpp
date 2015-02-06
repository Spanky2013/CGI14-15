#include "Triangle.hpp"
#include <vector>
#include "glm/glm/glm.hpp"

Triangle::Triangle(void)
{
}


Triangle::~Triangle(void)
{
}


Triangle::Triangle(glm::vec3 first, glm::vec3 second, glm::vec3 third){
	this->fir = first;
	this->sec = second;
	this->thi = third;	

	float x = (first.x+second.x+third.x)/3;
	float y = (first.y+second.y+third.y)/3;
	float z = (first.z+second.z+third.z)/3;

	this->midPoint = glm::vec3(x,y,z);
}

void Triangle::adNormals(glm::vec3 n1,glm::vec3 n2,glm::vec3 n3){
	this->firN = n1;
	this->secN = n2;
	this->thiN = n3;

	this->normal = glm::normalize(n1+n2+n3);

}