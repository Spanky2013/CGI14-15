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
	this->inv_dir = glm::vec3(1/dir.x,1/dir.y,1/dir.z);
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
