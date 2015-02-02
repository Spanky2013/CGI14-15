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
}