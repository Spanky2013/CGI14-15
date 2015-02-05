#include "Scene.hpp"

#include <string>
#include <iostream>

#include "GLSLShader.hpp"

#define GLM_FORCE_RADIANS
#include "glm\glm\gtx\transform.hpp"

using namespace glm;
using namespace std;

Scene::Scene(){
	std::vector<TriMesh> sceneObjects = std::vector<TriMesh>();
	triangles = std::vector<Triangle>();
};

void Scene::createScene(Scene scene){
	scene.AddObject("meshes/bialetti.off",0);
	scene.AddObject("meshes/bialetti.off",0);
	scene.AddObject("meshes/quad.off",0);		
};

void Scene::AddObject(std::string modelName, float off){
	TriMesh mesh;
	mesh.loadOff(modelName,off);

	mesh;

	sceneObjects.push_back(mesh);
};

void Scene::RenderScene(static GLSLShader shader, glm::mat4 modelMatrix, glm::mat4 projectionMatrix, static glm::mat4 cameraMatrix, LightSource lightSource,
				 Material material){

	shader.bind();

	for each(TriMesh mesh in sceneObjects) {		
		// TODO: Apply the model matrix for each mode to the model view matrix
	  shader.setUniform("modelView", cameraMatrix * modelMatrix);
      shader.setUniform("normalMatrix", glm::transpose(glm::inverse(cameraMatrix*modelMatrix)));
      shader.setUniform("modelViewProjection", projectionMatrix*cameraMatrix*modelMatrix);
      shader.setUniform("lighting", true);
	  shader.setUniform("showTexture", false);
	  shader.setUniform("pi", glm::pi<float>());

	  shader.setUniform("lightSource.position", lightSource.position);
	  shader.setUniform("lightSource.ambient", lightSource.ambient);
	  shader.setUniform("lightSource.diffuse", lightSource.diffuse);
	  shader.setUniform("lightSource.specular", lightSource.specular);
	  shader.setUniform("material.ambient", material.ambient);
	  shader.setUniform("material.diffuse", material.diffuse);
	  shader.setUniform("material.specular", material.specular);
	  shader.setUniform("material.shininess", material.shininess);

	   mesh.draw();
	   mesh.computeNormals();
       mesh.computeSphereUVs();

	   for each(Triangle t in mesh.triangles) {
		 triangles.push_back(t);
	   };
	}
	shader.unbind();	
};