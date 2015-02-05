#include <vector>
#include "KDNode.hpp"
#include "BoundingBox.hpp"
#include "RayTraceHelper.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

KDNode::KDNode()
{
}
KDNode::~KDNode()
{
}

KDNode* KDNode::build(std::vector<Triangle> triangles, int depth){
	KDNode* node = new KDNode();
	node->triangles = triangles;
	node->left = NULL;
	node->right = NULL;
	node->bbox = BoundingBox();

	//Sind wir in einem Blatt ohne Triangle
	if(triangles.size() == 0){
		return node;
	}

	node->bbox = BoundingBox::get_bounding_box(triangles);

	//Sind wir in einem Blatt mit genau einem Triangle
	if(triangles.size() == 1){		
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->triangles = std::vector<Triangle>();
		node->right->triangles = std::vector<Triangle>();
		return node;
	}

	glm::vec3 midPoint = node->bbox.midPoint;

	//Jetzt aufteilen

	std::vector<Triangle> leftTris;
	std::vector<Triangle> rightTris;
	int axis = node->bbox.get_longest_axis();
	
	for(int i = 0; i < triangles.size(); i++){
		glm::vec3 currentMidPoint = KDNode::get_midPoint_tr(triangles[i]);
		switch (axis){
		case 0: // x-Axis	
			if(midPoint.x >= currentMidPoint.x){
				rightTris.push_back(triangles[i]);
			}else{
				leftTris.push_back(triangles[i]);
			}
			break;
		case 1: // y-axis
			if(midPoint.y >= currentMidPoint.y){
				rightTris.push_back(triangles[i]);
			}else{
				leftTris.push_back(triangles[i]);
			}
			break;
		case 2: //z-Axis
			if(midPoint.z >= currentMidPoint.z){
				rightTris.push_back(triangles[i]);
			}else{
				leftTris.push_back(triangles[i]);
			}
			break;
		}
	}

	//längste Achse ist nicht unbedingt die beste
	//Falls Achse nichts bringt
	/*if((leftTris.size() == 0 && rightTris.size() > 0) || (leftTris.size() > 0 && rightTris.size() == 0)){
		leftTris.clear();
		rightTris.clear();
		axis = (axis + 1) % 3;
		for(int i = 0; i < triangles.size(); i++){
			glm::vec3 currentMidPoint = KDNode::get_midPoint_tr(triangles[i]);
			switch (axis){
				case 0: // x-Axis	
					if(midPoint.x >= currentMidPoint.x){
						rightTris.push_back(triangles[i]);
					}else{
						leftTris.push_back(triangles[i]);
					}
					break;
				case 1: // y-axis
					if(midPoint.y >= currentMidPoint.y){
						rightTris.push_back(triangles[i]);
					}else{
						leftTris.push_back(triangles[i]);
					}
					break;
				case 2: //z-Axis
					if(midPoint.z >= currentMidPoint.z){
						rightTris.push_back(triangles[i]);
					}else{
						leftTris.push_back(triangles[i]);
					}
					break;
			}
		}
	
		//Falls zweite Achse auch nichts gebracht hat
		if((leftTris.size() == 0 && rightTris.size() > 0) || (leftTris.size() > 0 && rightTris.size() == 0)){
			leftTris.clear();
			rightTris.clear();
			axis = (axis + 1) % 3;
			for(int i = 0; i < triangles.size(); i++){
				glm::vec3 currentMidPoint = KDNode::get_midPoint_tr(triangles[i]);
				switch (axis){
					case 0: // x-Axis	
						if(midPoint.x >= currentMidPoint.x){
							rightTris.push_back(triangles[i]);
						}else{
							leftTris.push_back(triangles[i]);
						}
						break;
					case 1: // y-axis
						if(midPoint.y >= currentMidPoint.y){
							rightTris.push_back(triangles[i]);
						}else{
							leftTris.push_back(triangles[i]);
						}
						break;
					case 2: //z-Axis
						if(midPoint.z >= currentMidPoint.z){
							rightTris.push_back(triangles[i]);
						}else{
							leftTris.push_back(triangles[i]);
						}
						break;
				}
			}
		}
	}*/

	// Haben die Dreiecke in beide Hälften aufgeteilt.
	// Jetzt kommt noch die Rekursion

	if(node->triangles.size() != leftTris.size())
		node->left = build(leftTris,depth+1);
	else{
		node->left = new KDNode();
		node->left->triangles = std::vector<Triangle>();
	}

	if(node->triangles.size() != rightTris.size())
		node->right = build(rightTris,depth+1);
	else{
		
		node->right = new KDNode();
		node->right->triangles = std::vector<Triangle>();
	}

	return node;

}


glm::vec3 KDNode::get_midPoint_tr(Triangle tri){
	glm::vec3 result,first,second,third;

	first = tri.fir;
	second = tri.sec;
	third = tri.thi;

	result.x = (first.x+second.x+third.x)/3;
	result.y = (first.y+second.y+third.y)/3;
	result.z = (first.z+second.z+third.z)/3;

	return result;
}

// time0 ist der Eintrittspunkt in die Box, time1 der Austrittspunkt
bool KDNode::hit_a_tr(KDNode* node, const Ray ray, float time1, float time0, RayTraceHelper& rtHelper){
	
	//erstma: hat er überhaupt in die bbox getroffen?
	if(node->bbox.hit_it(ray, time0, time1)){
		bool hit_tr = false;
		glm::vec3 hit_pt, local_hit_pt, normal;

		bool hit_left, hit_right;
		if(node->left->triangles.size() > 0 || node->right->triangles.size() > 0 ){
			hit_left = hit_a_tr(node->left, ray, time1, time0, rtHelper);
			hit_right = hit_a_tr(node->right, ray,time1,time0, rtHelper);
			return hit_left || hit_right;
		} else {
		// Wir sind in ienem Blatt
			for(int i = 0; i < node->triangles.size(); i++){
				//Treffen wir in diesem Blatt ein Triangle
				if(hit_ray_tr(ray, node->triangles[i],time1, time0)){
					hit_tr = true;
					//time0 = time1;
					hit_pt = hitPt_ray_tr(ray, node->triangles[i]);
					normal = hitNr_ray_tr(ray.dir, node->triangles[i]);
				}
			}
			if(hit_tr){//Wir haben gerade ein Triangle getroffen und geben jetzt die wichtigen Dinge zurück
				rtHelper.intersectionPoint = hit_pt;
				rtHelper.normalAtIntSec = normal;
				return true;
			}
		}
	}else{
		return false;
	}
}

// Erster Eintrag ist time0(eintritt in die BBox) 
// Zweiter Eintrag ist time1 (austritt aus derBBox)
// Wenn time0 > time 1 dann gibt es keinen Schnittpunkt
/*glm::vec2 KDNode::get_times(Ray ray){
	if(this->triangles.size() > 0){
		return glm::vec2(this->bbox.get_times(ray));
	}else{
		return glm::vec2(1.f,0.f);
	}
}*/

//Hat der Ray in der BBox auch dieses Dreieck getroffen?
//TODO: wozu t und tmin?
bool KDNode::hit_ray_tr(Ray ray, Triangle tri, float t, float tmin){
	bool res = false;
	
	glm::vec3 first,second,third, s, e1, e2, p, q;
	first = tri.fir;
	second = tri.sec;
	third = tri.thi;
	s = ray.src-first;
	e1 = second-first;
	e2 = third-first;
	p = glm::cross(ray.dir,e2);
	q = glm::cross(s,e1);

	float mul = 1/(glm::dot(p,e1));

	float u = mul*glm::dot(p,s);
	float v = mul*glm::dot(q,ray.dir);

	if( u >= 0 && v >= 0 && u+v<=1){
		this->hitPoint.x = (1-u-v)*first.x + u*second.x + v*third.x;
		this->hitPoint.y = (1-u-v)*first.y + u*second.y + v*third.y;
		this->hitPoint.z = (1-u-v)*first.z + u*second.z + v*third.z;

		res = true;
	}else{
		res = false;
	}

	return res;
}

//Wo hat der Ray dieses Dreieck getroffen? Darf nur aufgerufen werden, 
//wenn auf dieses Triangle bereits hit_ray_tr mit true aufgerufen wurde
glm::vec3 KDNode::hitPt_ray_tr(Ray ray, Triangle triangle) const{
	return hitPoint;
}

//gibt die Normale des getroffenen Dreiecks zurück
glm::vec3 KDNode::hitNr_ray_tr(glm::vec3 dir, Triangle triangle) const{
	glm::vec3 result = glm::normalize(
				glm::cross(
					(triangle.sec - triangle.fir),
					(triangle.thi - triangle.fir)
				)
			);

	//TODO weiß nicht ob das richtig ist...
	//------------------------------
	//Normale vielleicht au fder falschen Seite des Dreiecks
	
	//result und dir haben die länge 1!
	float h = glm::dot(result,dir);
	// Wenn der Winkel kleiner al 90° ist muss die normale umgedreht werden.
	if(acos(h) < M_PI/2){
		result = -1.f*result;
	}
	//------------------------------

	
	return result;
}