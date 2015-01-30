#include "Ray.hpp"
#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/unsigned_int.hpp"

Ray::Ray(void)
{
}

Ray::Ray(glm::vec3 source, glm::vec3 direction){
	this->dir = direction;
	this->src = source;
}

Ray::~Ray(void)
{
}


glm::vec3 Ray::posAt(float time) const{
	glm::vec3 result = this->src + time*this->dir;
	return result;
}
