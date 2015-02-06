/* ----------------------------------------------------------------
   name:           Texture.hpp
   purpose:        cg1_ex4 ws2014/15 texturing tutorial
   version:	   SKELETON CODE
   TODO:           nothing (see Texture.cpp)
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#pragma once

#include <string>

#include "glm/glm.hpp"
#include "light_material.h"
#include "KDNode.hpp"
#include "Scene.hpp"

struct LightSource;
struct Material;

namespace Common{

  void loadShaders();
  // common keyboard callback
  void keyPressed(unsigned char key, int x, int y);
};

namespace Texture{
  
  extern int menuOptions[];
  extern std::string menuText[];
  extern int numOptions;
  extern glm::vec2 previousMouse; // previous mouse position

  void display(void);
  void reshape(int width, int height);
  void mousePressed(int button, int state, int x, int y);
  void mouseDragged(int x, int y);
  void mouseMoved(int x, int y);
  void menu(int value);
};

namespace World{

  extern int menuOptions[];
  extern std::string menuText[];
  extern int numOptions;
  extern glm::vec2 previousMouse; // previous mouse position
  extern LightSource lightSource;
  extern std::vector<LightSource> lightSources; 
  extern Material material;

  static KDNode kdTree;

  void display(void);
  void reshape(int width, int height);
  void mousePressed(int button, int state, int x, int y);
  void mouseDragged(int x, int y);
  void menu(int value);

  void setLight();
  void setMaterial();


  void raytrace(const float x,const float y, const float subsampler);
  std::vector<Ray> createPrimaryRays(float x, float y, float subsampling);
  bool intersectTriangle(std::vector<Triangle> triangles, Ray rays, RayTraceHelper& rth);
  Ray getRay(int x, int y, float xPixel, float yPixel);
};	