/* ----------------------------------------------------------------
   name:           Texture.cpp
   purpose:        cg1_ex4 ws2014/15 texturing tutorial
   version:	   SKELETON CODE
   TODO:           see XXX
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#ifdef __APPLE__ 
  #include <OpenGL/gl.h>
#include <GL/freeglut.h>
#elif _WIN32
  #include "win32/glew.h"
#include "win32/freeglut.h"
#else
  #include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include <time.h>
#include <string>
#include <iostream>

#include "glm/glm/glm.hpp"
#define GLM_FORCE_RADIANS
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "Context.hpp"
#include "Texture.hpp"
#include "TriMesh.hpp"
#include "Image.hpp"
#include "GLSLShader.hpp"
#include "light_material.h"
#include "Ray.hpp"
#include "RayTraceHelper.hpp"


using namespace glm;
using namespace std;

static TriMesh mesh;
static TriMesh mesh2;
static TriMesh mesh1;

// full screen quad
static TriMesh quad("data/quad.off"); // do not center and unitize

#define PI  3.14159265358979323846264338327950288f
#define RADIANS(x) (((x)*PI)/180.0f)

// current state of mouse action
enum DragMode {
	ROTATE, SHIFT_XY, SHIFT_Z, SCALE, NO_DRAG, DRAW, ERASE
};
static DragMode drag = NO_DRAG;

static bool showTexture= true;
static bool textureCorrection= true;
static bool lighting= true;
static bool showCoordinates= true;
static bool showOrigin= true;
static bool environmentMapping= false;
static bool moveEnvironment= false;
static bool drawMesh= true;
static bool drawRect= false;
static bool silhouette = false;
static bool rayTracer = false;

static GLuint modulation= GL_MODULATE;
static GLuint wrap= GL_CLAMP_TO_BORDER;

static mat4 cameraMatrix;
static mat4 rotation= mat4(1); // current rotation of object
static vec3 shift= vec3(0); // offset
static float scaling= 1; // scale

static vec2 screen;  // screen size
static float fov= 60.0; // field of view
// camera setup
// camera position
static GLfloat cameraZ;
// cameraZ in original sphere map setup
static GLfloat cameraZMap= 0;
// near and far plane
static GLfloat nearPlane;
static GLfloat farPlane;

static Image texture;
static Image rayTexture;

static std::vector<glm::vec3> intPts; 

static vec3 cursor= vec3(1,1,1);

static GLSLShader quadShader;
static GLSLShader normQuadShader;
static GLSLShader texturingShader;
static GLSLShader sphereMapShader;
static GLSLShader silhouetteShader;

/*************************************************************************************/

// load Shaders
// XXX: NEEDS TO BE IMPLEMENTED
void Common::loadShaders(){
  // XXX

	normQuadShader.loadVertexShader("shaders/quad.vert");
	normQuadShader.loadFragmentShader("shaders/quad.frag");
	normQuadShader.bindVertexAttrib("position", TriMesh::attribVertex);
	normQuadShader.link();

	quadShader.loadVertexShader("shaders/quadShader.vert");
	quadShader.loadFragmentShader("shaders/quadShader.frag");
	quadShader.loadFragmentShader("shaders/blinnPhongReflection");
	quadShader.bindVertexAttrib("position", TriMesh::attribVertex);
	quadShader.link();

	sphereMapShader.loadVertexShader("shaders/sphere.vert");
	sphereMapShader.loadFragmentShader("shaders/sphere.frag");
	sphereMapShader.loadFragmentShader("shaders/blinnPhongReflection");
	sphereMapShader.bindVertexAttrib("position", TriMesh::attribVertex);
	sphereMapShader.bindVertexAttrib("normal", TriMesh::attribNormal);
	sphereMapShader.bindVertexAttrib("texCoord", TriMesh::attribTexCoord);
	sphereMapShader.link();

	texturingShader.loadVertexShader("shaders/textur.vert");
	texturingShader.loadFragmentShader("shaders/textur.frag");
	texturingShader.loadFragmentShader("shaders/blinnPhongReflection");
	texturingShader.bindVertexAttrib("position", TriMesh::attribVertex);
	texturingShader.bindVertexAttrib("normal", TriMesh::attribNormal);
	texturingShader.bindVertexAttrib("texCoord", TriMesh::attribTexCoord);
	texturingShader.link();

	silhouetteShader.loadVertexShader("shaders/silhouette.vert");
	silhouetteShader.loadFragmentShader("shaders/silhouette.frag");
	silhouetteShader.bindVertexAttrib("position", TriMesh::attribVertex);
	silhouetteShader.bindVertexAttrib("normal", TriMesh::attribNormal);
	silhouetteShader.link();
  // END XXX
}


// calculate cursor position
// XXX: NEEDS TO BE IMPLEMENTED
static void updateCursor(int x, int y){

  // XXX

	float u = x/screen.x;
	float v = 1 - y/screen.y;
	float ny = cos((1.f - v) * glm::pi<float>());
	float theta = (u - 0.5f) * (2 * glm::pi<float>());
	float nx = sin(theta);
	float nz = cos(theta);
	double l = glm::sqrt((1.f * 1.f - ny * ny) / (nx * nx + nz * nz));
	cursor = vec3(l*nx, ny, l*nz);

  // END XXX
}

static void reset(){

  rotation= mat4(1); // current rotation of object
  cameraZMap= 0;
  shift= vec3(0); // offset
  fov= 60.0;
}

void Common::keyPressed(unsigned char key, int x, int y){

  switch (key){

  case 'q':
  case 'Q':
    exit(EXIT_SUCCESS);
    break;
	  
  case 'r':
    reset();
    break;

  case 's':
    scaling*=0.9f;
    break;

  case 'S':
    scaling*=1.1f;
    break;

  case 'c':
    cameraZMap+=0.1f;
    break;
    
  case 'C':
    cameraZMap-=0.1f;
    break;
	 
  default: 
    break;
  }

  Context::displayWorldWindow();
}

// -------------------------------------------------------
// TEXTURE WINDOW
// -------------------------------------------------------

int Texture::menuOptions[]=  {0, 17, 18, 0, 1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 10, 11, 0, 12, 13, 14, 15, 16, 0, 19, 20, 21, 22, 23, 24, 25, 26, 0, 27, 28}; 
string Texture::menuText[]= {"TOOLS:", "    Pen", "    Eraser", 
			     "SPHERICAL TEXTURES:", "    Earth", "    Earth (ice)", "    Earth (night)", "    Saturn", "    Marble", 
			     "ENVIRONMENT TEXTURES:", "    St Peters dome", "    Uffizium", "    Supernova", "    Landscape", "    Forest", "    Phong Light",
			     "MISC TEXTURES", "    Checkerboard", "    Grid1", "    Grid2",  "    Spectrum",  "    Nemo",
			     "FILTERING", "    mag: NEAREST",  "    mag: LINEAR", "    min: NEAREST" , "    min: LINEAR", "    min: NEAREST_MIPMAP_NEAREST  ", "    min: LINEAR_MIPMAP_NEAREST", "    min: NEAREST_MIPMAP_LINEAR", "    min: LINEAR_MIPMAP_LINEAR",
			   "SILHOUETTES", "    narrow", "    broad"};

int Texture::numOptions= sizeof(Texture::menuOptions)/sizeof(Texture::menuOptions[0]);

string textures[]= {"", "data/earthcyl2.ppm", "data/earth2.ppm", "data/earthlights.ppm", "data/saturncyl1.ppm", "data/marble.ppm", "data/stpeters.ppm", "data/uffizi.ppm", "data/supernova.ppm", "data/test5b.ppm", "data/test7b.ppm", "data/test6b.ppm", "data/checker2.ppm", "data/test3b.ppm", "data/test4b.ppm", "data/test2b.ppm", "data/test8b.ppm", "data/silhouette.ppm"};

vec2 Texture::previousMouse; // previous mouse position

void Texture::reshape(int width, int height){

  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();

  //// Set the viewport to be the entire window
  //glViewport(0, 0, width, height);

  //gluOrtho2D(0, width, 0, height);

  //screen= vec2(width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, width, height);

  gluOrtho2D(0, width, 0, height);

  screen= vec2(width, height);
}

// display texture
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::display(void){

  // setup model matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.5, 0.5, 0.5, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  	if(rayTracer){
		normQuadShader.bind();
		rayTexture.bind();
		quad.draw();
		rayTexture.unbind();
		normQuadShader.unbind();

	}else{
		mesh1.draw();
		mesh.draw();
		mesh2.draw();
	}


  glutSwapBuffers();
//    glPopAttrib();

}

void Texture::mousePressed(int button, int state, int x, int y){

  if(button == GLUT_DOWN) 
	  previousMouse= vec2(x, y);
}

// mouse dragged callback
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::mouseDragged(int x, int y){

  // paint on texture
  // XXX

  /*if(drag == DragMode::DRAW) {
		texture.paint((x / screen.x) * texture.getWidth(), ((screen.y - y) / screen.y) * texture.getHeight());
  } else {
		texture.erase((x / screen.x) * texture.getWidth(), ((screen.y - y) / screen.y) * texture.getHeight());
  }*/
  // END XXX

  //updateCursor(x, y);		
		//
  //previousMouse= vec2(x, y);

   // Context::displayTextureWindow();
//Context::displayWorldWindow();
}

void Texture::mouseMoved(int x, int y){

  //if (x > 0 && x < screen.x && y > 0 && y < screen.y){
  //  updateCursor(x, y);
  //}
}

// menu callback
// XXX: NEEDS TO BE IMPLEMENTED
void Texture::menu(int value){

  switch(value){
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:	
  case 8:
  case 9:	
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
  case 16:
    //texture.load(textures[value]);
    //texture.generateTexture();
    //if(value<6) environmentMapping= false;
    //else if(value<13) environmentMapping= true;
    break;
  case 17:
    //drag= DRAW;
    break;
  case 18:
    //drag= ERASE;
    break;

    // add cases for texture filtering
    // XXX

	case 19:
		texture.setMagFilter(GL_NEAREST);
		break; 
	case 20:
		texture.setMagFilter(GL_LINEAR);
		break;
	case 21:
		texture.setMinFilter(GL_NEAREST);
		break; 
	case 22:
		texture.setMinFilter(GL_LINEAR);
		break; 
	case 23:
		texture.setMinFilter(GL_NEAREST_MIPMAP_NEAREST);
		break; 
	case 24:
		texture.setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
		break;
	case 25:
		texture.setMinFilter(GL_NEAREST_MIPMAP_LINEAR);
		break;
	case 26:
		texture.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
		break;
	case 27:
		texture.load(textures[value-10]);
		texture.generateTexture();
		if(value<6) environmentMapping= false;
		else if(value<13) environmentMapping= true;
		break;

    // END XXX
  default: 
    break;
  }

      //Context::displayTextureWindow();
//Context::displayWorldWindow();
}

// -------------------------------------------------------
// WORLD-SPACE WINDOW
// -------------------------------------------------------

//int World::menuOptions[]= {24, 25, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
//			   0, 15, 16, 17, 18, 19, 20, 21, 22, 23};
//string World::menuText[]= {"    reset","Draw Objects", "MODEL", "    Plane", "    Spiky Sphere", "    Car", "    Bunny", "    Cone", "    Cow", "    Cowboy Hat", "    Dragon", "    Chess", "    Temple", "    Cup", "    Space Shuttle", "    Sphere", "    None",
//			   "RENDERING", "    Lighting on/off", "    Texture on/off", "    Coordinate System on/off", "    Origin on/off", 
//			   "    Texture Coordinate Correction on/off  ", "    Texture Mode (WRAP/CLAMP) ", "    Environment mapping on/off", "    Move object/environment", "    SilhouetteRendering"};
//
//int World::numOptions= sizeof(World::menuOptions)/sizeof(World::menuOptions[0]);
int World::menuOptions[]= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
string World::menuText[]= {"Draw Objects","Draw None", "KD-Calc", "Raytrace Scene", "    Lighting on/off", "    Texture on/off", "    Coordinate System on/off", "    Origin on/off","Points on/off", "Create Scene"};
			      // Texture Coordinate Correction on/off  ", "    Texture Mode (WRAP/CLAMP) ", "    Environment mapping on/off", "    Move object/environment", "    SilhouetteRendering"};

int World::numOptions= sizeof(World::menuOptions)/sizeof(World::menuOptions[0]);

static string models[]= {"","", "data/quad.off", "data/4cow.off", "data/auto3.off", "data/bunny2.off", "data/cone.off", "data/cow.off", "data/cowboyhut.off", "data/MEGADRACHE.off", "data/Schachfigur.off", "data/tempel.off", "data/tasse.off", "data/spaceshuttle.off", "data/sphere.off"};

bool showPoints = false;

vec2 World::previousMouse;
LightSource World::lightSource;
Material World::material;
static Scene scene;
void World::reshape(int width, int height){

    // setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    //cameraZ= 1 / tan(fov/180.0);
	cameraZ = 1;
    // near and far plane
    nearPlane= cameraZ/10.0f;
    farPlane= cameraZ*10.0f;
    
    gluPerspective(fov, (float)width/(float)height, nearPlane, farPlane);
    
    //position the camera at (0,0,cameraZ) looking down the
    //negative z-axis at (0,0,0)
    cameraMatrix= lookAt(vec3(0.5, 0.5, cameraZ), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

    screen= vec2(width, height);
}



// display callback
// XXX: NEEDS TO BE IMPLEMENTED
void World::display(void){

  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf(&cameraMatrix[0][0]);

  glEnable(GL_DEPTH_TEST);

  // shift object
  glTranslatef(shift.x, shift.y, shift.z);

  // rotate Object
  glMultMatrixf(&rotation[0][0]);

  //show coordinate system
  if(showCoordinates){
    glBegin(GL_LINES);
    glColor3ub(255, 0, 0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(2.0, 0.0, 0.0);

    glColor3ub(0, 255, 0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 2.0, 0.0);

    glColor3ub(0, 0, 255);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 2.0);
    glEnd();
  }		

  // show center of spherical mapping
  if(showOrigin){
    glColor3f(1.0, 1.0, 0.0);
    glPushMatrix();
   
    glutSolidSphere(0.05f, 20, 20);
    glPopMatrix();
  }

  if(showPoints) {
	  glColor3f(1.0,0.0,5.0);
	  glPushMatrix();
	  glBegin(GL_POINTS);

	  //TODO render IntersectionPoints
	  for(unsigned int i = 0; i < intPts.size();i++){   
		glm::vec3 point = intPts[i];
		glVertex3f(point.x, point.y, point.z);
	  }
	  glEnd();
	  glPopMatrix();
  }

  glScalef(scaling, scaling, scaling);
	
     if(!drawMesh){
     glutSwapBuffers();
     return;
     }

// draw the geometry

	mat4 modelMatrix= translate(mat4(1), vec3(shift.x, shift.y, shift.z));
	modelMatrix *= rotation;
	//modelMatrix= scale(modelMatrix, vec3(scaling));  

	mat4 projectionMatrix;
	glGetFloatv(GL_PROJECTION_MATRIX, &projectionMatrix[0][0]);

	if(drawMesh){ // draw a textured quad
	//Scene::RenderScene(texturingShader,modelMatrix, projectionMatrix, cameraMatrix, lightSource,material);// pass matrices and flags to shader
    // XXX

     //INSERT YOUR CODE HERE     
	  texturingShader.bind();
	  texturingShader.setUniform("modelView", cameraMatrix * modelMatrix);
      texturingShader.setUniform("normalMatrix", glm::transpose(glm::inverse(cameraMatrix*modelMatrix)));
      texturingShader.setUniform("modelViewProjection", projectionMatrix*cameraMatrix*modelMatrix);
      texturingShader.setUniform("lighting", true);
	  texturingShader.setUniform("showTexture", false);
	  texturingShader.setUniform("pi", glm::pi<float>());

	  texturingShader.setUniform("lightSource.position", lightSource.position);
	  texturingShader.setUniform("lightSource.ambient", lightSource.ambient);
	  texturingShader.setUniform("lightSource.diffuse", lightSource.diffuse);
	  texturingShader.setUniform("lightSource.specular", lightSource.specular);
	  texturingShader.setUniform("material.ambient", material.ambient);
	  texturingShader.setUniform("material.diffuse", material.diffuse);
	  texturingShader.setUniform("material.specular", material.specular);
	  texturingShader.setUniform("material.shininess", material.shininess);
	   //texture.bind();
	   mesh.draw();
	   mesh1.draw();
	   mesh2.draw();
	   //texture.unbind();
	 texturingShader.unbind();
	}    
  glutSwapBuffers();
}

void World::mousePressed(int button, int state, int x, int y){

  int modifier;

  switch(button){
  case GLUT_LEFT_BUTTON:
    if(state == GLUT_DOWN){
      previousMouse= vec2(x, y);
      modifier = glutGetModifiers();
      if(modifier & GLUT_ACTIVE_CTRL)
	drag = SHIFT_XY;
      else if(modifier & GLUT_ACTIVE_SHIFT)
	drag = SHIFT_Z;
      else
	drag = ROTATE;
    }
    else if(state == GLUT_UP){
      drag = NO_DRAG;
    }
    break;
  default: break;
  }
Context::displayWorldWindow();
}

void World::mouseDragged(int x, int y){

  vec2 v= (vec2(x,y) - previousMouse) / screen;
  
	switch(drag){
	case ROTATE:
	  if(length(v)==0) break;
	  rotation= rotate(mat4(1), RADIANS(180 * length(v)), normalize(vec3(v.y, v.x, 0))) * rotation;
		break;
	case SHIFT_XY:
	  if(false){
	    shift.x+= 3.3f*v.x;
	    shift.y-= 3.3f*v.y;	
	  }
	  else{
	    shift.x+= 3.3f*v.x;
	    shift.y-= 3.3f*v.y;
	  }  
		break;
	case SHIFT_Z:
	 shift.z+= 3.3f*sign(dot(v, vec2(1,1))) * length(v);
	  break;
	default:
	  break;
	}
	previousMouse= vec2(x, y);

Context::displayWorldWindow();
}

// menu callback
// XXX: NEEDS TO BE IMPLEMENTED
void World::menu(int value){
  double tstart;
  std::vector<Triangle> sum;
  switch(value){
  case 0:
    // load rectangle
    mesh.loadOff("meshes/bialetti.off");
   // mesh.center();
    //mesh.unitize();
    mesh.computeNormals();
    mesh.computeSphereUVs();
	mesh1.loadOff("meshes/bialetti.off",0.1f);
	//mesh1.center();
    //mesh1.unitize();
    mesh1.computeNormals();
    mesh1.computeSphereUVs();
	mesh2.loadOff("meshes/quad.off");
	//mesh2.center();
    //mesh2.unitize();
    mesh2.computeNormals();
    mesh2.computeSphereUVs();
    drawRect= true;
    break;
  case 1:
    drawMesh= false;
    break;
  case 2: 
	 //mesh.loadOff("meshes/bunny.off");	
	 tstart = clock();
	 sum = std::vector<Triangle>(mesh.triangles);
	 sum.insert(sum.end(), mesh1.triangles.begin(), mesh1.triangles.end());	  
	  sum.insert(sum.end(), mesh2.triangles.begin(), mesh2.triangles.end());
	  kdTree = *KDNode::build(sum, 0);
	  cout << "end KD after " << ((clock() - tstart)/CLOCKS_PER_SEC) << endl;;
	  break;
  case 3: 
	  rayTracer = true;
	  tstart = clock();
	  raytrace(screen.x,screen.y);
	  cout << "end raytrace after " << ((clock() - tstart)/CLOCKS_PER_SEC) << endl;	  
	  drawRect = true;
	  break;
  case 4:
    lighting= !lighting;
    break;
  case 5:
    showTexture= !showTexture;
    break;
  case 6:
    showCoordinates= !showCoordinates;
    break;
  case 7:
    showOrigin= !showOrigin;
    break;
  case 8:
	showPoints= !showPoints;
    break;
  case 9:
	 Scene::createScene(scene);
	 break;
  case 10:
  case 11:
  case 12:
  case 13:
    mesh.correctTexture(textureCorrection);
      if(models[value] == "data/bunny2.off" || models[value] == "data/cow.off" || models[value] == "data/cone.off"){ mesh.setWinding(TriMesh::CCW); }
    else mesh.setWinding(TriMesh::CW);
    mesh.loadOff(models[value]);
    mesh.center();
    mesh.unitize();
    mesh.computeNormals();
    mesh.computeSphereUVs();
    drawRect= false;
    drawMesh= true;
    break;
  case 19:
    textureCorrection= !textureCorrection;
    // enable/disable texture correction in Image (not obligatory, but useful for debugging)
    textureCorrection= !textureCorrection;
    mesh.correctTexture(textureCorrection);
    mesh.reload();
    mesh.center();
    mesh.unitize();
    mesh.computeNormals();
    mesh.computeSphereUVs();
    break;
  case 20:
    // set texture wrapping in Image (not obligatory, but useful for debugging)
    if(wrap==GL_REPEAT) wrap= GL_CLAMP_TO_BORDER;
    else wrap= GL_REPEAT;
    texture.setWrap(wrap);
    break;
    break;
  case 21:
    environmentMapping= !environmentMapping;
    if(!environmentMapping) moveEnvironment= false;
    break;
  case 22:
    reset();
    moveEnvironment= !moveEnvironment;
    break;
  case 23:
	  silhouette = !silhouette;
	  if(environmentMapping) environmentMapping = !environmentMapping;
	  break;
  case 24:
    reset();
    break;
  default:
    break;
  }
 Context::displayWorldWindow();
}

void World::setLight(){
  lightSource.position= vec4(1.f,0.f,0.f,1.f);
  lightSource.ambient= vec4(0.1f,0.1f,0.1f,1.f);
  lightSource.diffuse= vec4(1.f,1.f,1.f,1.f);
  //lightSource.specular= vec4(1,1,1,1);
}

// material
void World::setMaterial(){
  material.ambient= vec4(1,1,1,1);
  material.diffuse= vec4(1,1,1,1);
  material.specular= vec4(1,1,1,1);
  material.shininess= 0.75;
}

// x is the amount of Pixels in one row of the image you wanna RayTrace
// y is the Amount of Pixels in one column of the image you wanna RayTrace
void World::raytrace(float x, float y){

	cout << "Scrren " << x << " mal " << y << endl;

	//std::vector<std::vector<glm::vec4>> image = std::vector<std::vector<glm::vec4>>();
	std::vector<glm::vec4> image;
	int blacks = 0;
	int whites = 0;

	for(int i = 0; i < x; i++){
		//cout << i;
		//image.push_back(std::vector<glm::vec4>());
		for(int j = 0; j < y; j++){
			Ray ray = getRay(i,j,x,y);
			//cout<< "Ray src(" << ray.src.x << "," << ray.src.y << "," << ray.src.z << ") dir(" << ray.dir.x << "," << ray.dir.y << "," << ray.dir.z << ")" << endl; 
			RayTraceHelper rth;

			float time0 = 0;
			float time1 = std::numeric_limits<float>::max();
			
			if(kdTree.hit_a_tr(&kdTree, ray, time1, time0, rth)){
				intPts.push_back(rth.intersectionPoint);
				//Wir treffen ein Triangle in der BBox, rth ist jetzt aktualisiert und hat den genauen Punkt und die Normale
				// hier mürde es dann mit der Rekusrion irgendwie losgehen
					
				//TODO wie kriegen wir jetzt die Farbe?
				//image[i].push_back(material.ambient);
				image.push_back(glm::vec4(1,0,0,0));
				whites++;
				
			}else{
				//Hintergrundfarbe Schwarz
				//image[i].push_back(glm::vec4(0,0,0,1));
				image.push_back(glm::vec4(0,0,0,1));
				blacks++;
			}

			
		}
	}

	cout << "Balck " << blacks << "; White " << whites << endl;
	//TODO die Farben -vektoren aus Image jetzt zu einer Textur umwandeln!
	rayTexture.load(image,x,y);
	rayTexture.generateTexture();

	Context::displayTextureWindow();
};

// (x,y) is the pixel you wanna shoot at
// xPixel is the amount of Pixels in one row of the image you wanna RayTrace (normaly x of the screen-onject))
// yPixel is the Amount of Pixels in one column of the image you wanna RayTrace (normaly y of the screen-onject))
Ray World::getRay(int x, int y, float xPixel, float yPixel){

	glm::vec3 origin = glm::vec3(0,0,0);

	

	//cout << origin.x << "," << origin.y << "," << origin.z << endl;

	float xPart, yPart;
	if(xPixel > 1){
		xPart = ((float) x) / (xPixel-1);
	}else{
		xPart = ((float) x) / (xPixel);
	}
	if(yPixel > 1){
		yPart = ((float) y) / (yPixel-1);
	}else{
		yPart = ((float) y) / (yPixel);
	}	

	//Bin mir nicht 100% sicher, ob die Berechnung 
	//der vier seiten wirklich korrekt ist, aber ich denke schon.
	// http://wiki.delphigl.com/index.php/Tutorial_Raytracing_-_Grundlagen_I
	// https://stackoverflow.com/questions/13078243/ray-tracing-camera

	
	float left, right, bottom, top;
	left = -(xPixel-1)/2.f;
	//cout << left << endl;
	right = (xPixel-1)/2.f;
	//cout << right << endl;
	top = (yPixel-1)/2.f;
	//cout << top << endl;
	bottom = -(yPixel-1)/2.f;
	//cout << bottom << endl;

	xPart = left + xPart * (right-left);
	//cout << xPart << endl;
	yPart = top + yPart * (bottom - top);	
	//cout << yPart << endl;

	glm::vec3 direction = glm::normalize(glm::vec3(xPart,yPart,cameraZ));

	glm::vec4 helper =  glm::inverse(cameraMatrix) * glm::vec4(origin, 1.f);
	origin.x = helper.x;
	origin.y = helper.y;
	origin.z = helper.z;
	helper = glm::inverse(cameraMatrix) * glm::vec4(direction, 0.f);
	direction.x = helper.x;
	direction.y = helper.y;
	direction.z = helper.z;

	if(x==0 && y == 0)
		cout << "origin " << origin.x << "," << origin.y << ","<< origin.z << endl;
	if(x==0 && y == 0)
		cout << "direction " << direction.x << "," << direction.y << ","<< direction.z << endl;

	return Ray(origin,glm::normalize(direction));
}