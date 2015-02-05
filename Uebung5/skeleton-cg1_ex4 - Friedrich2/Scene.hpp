#include <memory>
#include <vector>

#include "glm\glm\glm.hpp"
#include "Ray.hpp"
#include "TriMesh.hpp"
#include "GLSLShader.hpp"
#include "light_material.h"
#include "Triangle.hpp"

struct LightSource;
struct Material;
extern LightSource lightSource;
extern Material material;

using namespace glm;


class Scene {
	 	public:
		
		TriMesh mesh;
		
	
		Scene(void);

		static void Scene::createScene(Scene scene);
		void Scene::AddObject(std::string modelName, float off);

		void RenderScene(static GLSLShader shader, glm::mat4 modelMatrix, glm::mat4 projectionMatrix, static glm::mat4 cameraMatrix, 
			LightSource lightSource, Material material);

		std::vector<Triangle> triangles;
		std::vector<TriMesh> sceneObjects;

};