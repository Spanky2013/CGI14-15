#include "RayTraceHelper.hpp"



RayTraceHelper::RayTraceHelper(void)
{
	this->normalAtIntSec = glm::vec3();
	this->intersectionPoint = glm::vec3();
	this->time = std::numeric_limits<float>::max();
}


RayTraceHelper::~RayTraceHelper(void)
{
}
