#include "Ray.hpp"
#include <vector>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtx/unsigned_int.hpp"

Ray::Ray(void)
{
}

//directionvector will be normalized
Ray::Ray(glm::vec3 source, glm::vec3 direction){
	this->dir = glm::normalize(direction);
	this->src = source;
	float invx = 1.f/dir.x;	
	float invy = 1.f/dir.y;
	float invz = 1.f/dir.z;

	this->inv_dir = glm::vec3(invx,invy,invz);
	sign[0] = (inv_dir.x < 0);
    sign[1] = (inv_dir.y < 0);
    sign[2] = (inv_dir.z < 0);
}

Ray::~Ray(void)
{
}


glm::vec3 Ray::posAt(float time) const{
	glm::vec3 result = this->src + time*this->dir;
	return result;
}
