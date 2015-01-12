/*
    21.12.2006 Andrew Nealen, Anders Adamson
    texturing in the context 
    of 'introduction to computer graphics' 
    winter term 2006/2007, assignment 4
    
    --------------------------------------
    | computer graphics                  |
    | technische universitaet berlin     |
    | www.cg.tu-berlin.de                |
    --------------------------------------
    
*/

#include &lt;math.h&gt;
#include &lt;stdio.h&gt;

#ifdef MACOSX
#include &lt;GLUT/glut.h&gt;
#include &lt;stdlib.h&gt;
#else
#include &lt;GL/glut.h&gt;
#include &lt;malloc.h&gt;
#endif

#include &lt;string.h&gt;
#include "glm.h"

#define GAP 3
#define PI 3.141592654
#define SCALE 80




typedef struct myVertex{
	GLfloat x,y,z; //Koordinaten des Punktes
	GLfloat nx,ny,nz; //Normale des Punktes
	GLfloat u,v; //Texturkoordinaten
	int adjacent_triangles[50]; //Indizes der anliegenden FlÃ¤chen
	int adjacent_number; //Anzahl der tatsÃ¤chlich anliegenden FlÃ¤chen
} vertex;

typedef struct myTriangle{
	int vertex_index[3]; //Indizes der Eckpunkte
	GLfloat normal[3]; //FlÃ¤chennormale
} triangle;
/***************************** global variables *******************************/

GLuint _mainWindow, _textureWindow, _worldWindow;
GLuint _width = 1024, _height = 512; // screen size

char _model = 'x';

float _origin[] = { 0.0f, 0.0f, 0.0f }, _pointer[3] = { 0.0f, 0.0f, 0.0f };

enum // current state of mouse action
{
	ROTATE, SHIFT_XY, SHIFT_Z, SCALE_VIEW, DRAW_POINTS, NO_DRAG
} _drag = NO_DRAG;

// Safe Mousepositions
GLfloat cursor[3] = {0,0,0};
GLfloat savedPos[2] = {0,0};
enum
{
	ON, OFF 
};

int _showNormals = OFF;
int _showTexture = ON;
int _showOrigin = OFF;
int _showCoordinates = ON;
int _doLighting = ON;
int _environmentMapping = OFF;

enum
{
	NORMAL, POINT
};

int _oldX, _oldY;
float _rotObjectX = 0.0, _rotObjectY = 0.0; // current rotation of object
float _shiftX = 0.0, _shiftY = 0.0, _shiftZ = 0.0; // offset
float _fovy = 60.0; // field of view

int _nVertices = 0, _nFaces = 0;
float *_vertices = NULL, *_normals = NULL, *_textures = NULL;
int *_faces;

static GLuint texName;
unsigned char *_textureImage = NULL;
int _texX, _texY;
float center[3];

vertex vertex_array[100000];
triangle triangle_array[100000];

void set_face_normal(int v1, int v2, int v3, int ind);
float get_center();
void draw_env_triangle(int index);
void draw_non_env_triangle(int index);
void draw_triangles();
void init_texture();
void draw_texture();
void print_triangle_list();
float get_abs(float value);
void draw_normals();
void set_vertex_normals();



/***************************** utility functions *******************************/

float get_abs(float value){
	if(value &lt; 0)
		return -value;
	else
		return value;
}

void add(float v1[3], float v2[3], float result[3])
{
	// v1, v2 are not changed
	result[0] = v1[0] + v2[0];
	result[1] = v1[1] + v2[1];
	result[2] = v1[2] + v2[2];
}

void sub(float v1[3], float v2[3], float result[3])
{
	// v1, v2 are not changed
	result[0] = v1[0] - v2[0];
	result[1] = v1[1] - v2[1];
	result[2] = v1[2] - v2[2];
}

void scalarProduct(float v[3], float s, float result[3])
{
	// v is not changed
	result[0] = v[0] * s;
	result[1] = v[1] * s;
	result[2] = v[2] * s;
}

void crossProduct(float v1[3], float v2[3], float result[3])
{
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

float dotProduct(float v1[3], float v2[3])
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

float length(float v[3])
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void normalize(float v[3], float result[3])
{
	// v is not changed
	float l;

	l = length(v);
	result[0] = v[0] / l;
	result[1] = v[1] / l;
	result[2] = v[2] / l;
}

void move(float v[3], float result[3])
{
	// copy v into result
	int i;

	for(i = 0; i &lt; 3; i++)
		result[i] = v[i];
}

void set_face_normal(int v1, int v2, int v3, int ind){
	
	float vec1[3];
	float vec2[3];
	float vec3[3];
	float vec_cross1[3];
	float vec_cross2[3];
	float vec_result[3];
	
	vec1[0] = vertex_array[v1].x;
	vec1[1] = vertex_array[v1].y;
	vec1[2] = vertex_array[v1].z;
	
	vec2[0] = vertex_array[v2].x;
	vec2[1] = vertex_array[v2].y;
	vec2[2] = vertex_array[v2].z;
	
	vec3[0] = vertex_array[v3].x;
	vec3[1] = vertex_array[v3].y;
	vec3[2] = vertex_array[v3].z;
	
	vec_cross1[0] = vec1[0]-vec2[0];
	vec_cross1[1] = vec1[1]-vec2[1];
	vec_cross1[2] = vec1[2]-vec2[2];
	
	vec_cross2[0] = vec3[0]-vec2[0];
	vec_cross2[1] = vec3[1]-vec2[1];
	vec_cross2[2] = vec3[2]-vec2[2];

	crossProduct(vec_cross1,vec_cross2,vec_result);
	normalize(vec_result,vec_result);
	triangle_array[ind].normal[0] = vec_result[0];
	triangle_array[ind].normal[1] = vec_result[1];
	triangle_array[ind].normal[2] = vec_result[2];
	
	//printf("Flaechennormale %d ist [%f,%f,%f]\n",ind,vec_result[0],vec_result[1],vec_result[2]);
}

void set_vertex_normals(){
	int i, j;
	GLfloat norm[3];

	for(i = 0; i &lt; _nVertices; i++){
		norm[0] = 0;
		norm[1] = 0;
		norm[2] = 0;
		for(j = 0; j &lt; vertex_array[i].adjacent_number;j++){
			norm[0] += triangle_array[vertex_array[i].adjacent_triangles[j]].normal[0];
			norm[1] += triangle_array[vertex_array[i].adjacent_triangles[j]].normal[1];
			norm[2] += triangle_array[vertex_array[i].adjacent_triangles[j]].normal[2];
			//printf("Vertex %d: x: %f, y: %f, z:%f\n",i,norm[0],norm[1],norm[2]);
		}
		normalize(norm,norm);
		vertex_array[i].nx = norm[0];
		vertex_array[i].ny = norm[1];
		vertex_array[i].nz = norm[2];
		//printf("Vertex %d Normale: x: %f, y: %f, z:%f\n",i,norm[0],norm[1],norm[2]);
	}
}

float get_center(){
	float x_max = vertex_array[0].x;
	float y_max = vertex_array[0].y;
	float z_max = vertex_array[0].z;
	float x_min = x_max;
	float y_min = y_max;
	float z_min = z_max;
	float scale = 0;

	int i = 1;
	while(i &lt; _nVertices){
		if(vertex_array[i].x &gt; x_max){
			x_max = vertex_array[i].x;
		}
		if(vertex_array[i].x &lt; x_min){
			x_min = vertex_array[i].x;
		}

		if(vertex_array[i].y &gt; y_max){
			y_max = vertex_array[i].y;
		}
		if(vertex_array[i].y &lt; y_min){
			y_min = vertex_array[i].y;
		}

		if(vertex_array[i].z &gt; z_max){
			z_max = vertex_array[i].z;
		}
		if(vertex_array[i].z &lt; z_min){
			z_min = vertex_array[i].z;
		}
		i++;
	}
	//printf("x_max: %f, x_min: %f, y_max: %f, y_min: %f, z_max: %f, z_min: %f\n", x_max, x_min, y_max, y_min, z_max, z_min);
	center[0] = (x_max + x_min)/2.0f;
	center[1] = (y_max + y_min)/2.0f;
	center[2] = (z_max + z_min)/2.0f;
	printf("Zentrum der Figur liegt bei [%f,%f,%f]\n",center[0],center[1],center[2]);
	printf("Groesse der Figur: %f, %f, %f\n",get_abs(x_max) + get_abs(x_min),get_abs(y_max) + get_abs(y_min),get_abs(z_max) + get_abs(z_min));
	scale = get_abs(x_max) + get_abs(x_min);
	if(get_abs(y_max) + get_abs(y_min) &gt; scale){
		scale = get_abs(y_max) + get_abs(y_min);
	}
	if(get_abs(z_max) + get_abs(z_min) &gt; scale){
		scale = get_abs(z_max) + get_abs(z_min);
	}
	return scale;
}

/*************************************************************************************/

void updatePointer(int x, int y)
{
	// XXX - insert code here
	float u,v,x_tmp,y_tmp;

	//Mauskoordinaten normieren auf (1,0)
	x_tmp=x;
	y_tmp=y;
	
	
	u=x_tmp/_texX;
	v=y_tmp/_texY;
	
	//Umrechnen in Kugelkoordinaten 
	cursor[0] = sin(v*PI)*cos(u*2*PI + PI / 2);
	cursor[1] = sin(v*PI)*sin(u*2*PI + PI / 2);
	cursor[2] = cos(v*PI);
}




void calculateTextureCoordinates(void)
{
	// XXX - insert code here
	int i = 0;
	while(i &lt; _nVertices){

		vertex_array[i].u = (PI + atan2(vertex_array[i].x-center[1],-vertex_array[i].z-center[0]))/(2*PI);

		vertex_array[i].v = atan2(sqrt((vertex_array[i].x-center[1])*(vertex_array[i].x-center[1])+(vertex_array[i].z-center[0])*(vertex_array[i].z-center[0])),-vertex_array[i].y-center[2])/PI;
		
		//printf("Vertex %d, u: %f, V: %f\n",i,vertex_array[i].u,vertex_array[i].v);
		i++;
	}

}

void readOffFile(char* filename)
{
	FILE		*file;
	char line[81];
	char* off = "OFF\0";
	int numVertices = 0;
	int numPolygons = 0;
	int numEdges = 0;
	float my_x, my_y, my_z;
	int my_temp, my_vertex1, my_vertex2, my_vertex3;
	int i;
	vertex* helper_vertex;
	float scale;
	
	// open an OFF file for reading
	printf("Reading OFF-file...\n", filename);
	
	file = fopen(filename, "r");
	if(file == NULL)
	{
		printf("konnte Datei nicht Ã¶ffnen\n");
		exit(1);
	}
	
	// XXX - insert code here

	// TODO: overread the OFF specifier in the first line, 
	// output error message and quit if missing
	if(!feof(file)){
		printf("geoeffnet\n");
		//fgets(line,80,file);
		fscanf_s(file,"%s",line,81);

		if(strcmp(off, line) != 0){
			printf("falsches Dateiformat (%s statt %s)\n",line, off);
			exit(1);
		}
	}
	else{
		printf("Datei zu kurz\n");
		exit(1);
	}

	// TODO: read model info, i.e. number of vertices etc.
	if(!feof(file)){
		fscanf_s(file,"%d",&amp;_nVertices);
		fscanf_s(file,"%d",&amp;_nFaces);
		fscanf_s(file,"%d",&amp;numEdges);
		printf("Vertices: %d, Polygons: %d, Edges: %d\n",_nVertices,_nFaces,numEdges);
	}
	else{
		printf("Datei zu kurz\n");
		exit(1);
	}
	// allocate data structures
	// vertexArray is helper structure only used to quickly
	// index vertices when reading the faces
	
	// TODO: read vertices
	// create a new Vertex object for each vertex and store it in the vertices list
	i = 0;
	while(i &lt; _nVertices){
		vertex_array[i].adjacent_number = 0;
		i++;
	}
	i = 0;
	while(i &lt; _nVertices){
		if(!feof(file)){
			fscanf_s(file,"%f",&amp;my_x);
			fscanf_s(file,"%f",&amp;my_y);
			fscanf_s(file,"%f",&amp;my_z);
			vertex_array[i].x = my_x;
			vertex_array[i].y = my_y;
			vertex_array[i].z = my_z;
			//printf("FÃ¼lle Vertex %d mit x: %f, y: %f, z: %f\n",i, my_x,my_y,my_z);
			i++;
		}
		else{
			printf("Datei zu kurz\n");
			exit(1);
		}
	}

	// TODO: Read faces
	// create a new Triangle object for each face and pass it the pointers to the
	// corresponding vertex objects, do not make copies!
	// store the triangle in the TrianglesList
	i = 0;
	while(i &lt; _nFaces){
		fscanf_s(file,"%d",&amp;my_temp);
		fscanf_s(file,"%d",&amp;my_vertex1);
		fscanf_s(file,"%d",&amp;my_vertex2);
		fscanf_s(file,"%d",&amp;my_vertex3);
		triangle_array[i].vertex_index[0] = my_vertex1;
		triangle_array[i].vertex_index[1] = my_vertex2;
		triangle_array[i].vertex_index[2] = my_vertex3;
		//printf("Scanne Dreiecke ... \n");
		//den Eckpunkten des Dreiecks sagen, dass es ein neues Nachbardreieck hat und welches dies ist
		//Vertex 1
		helper_vertex = &amp;vertex_array[my_vertex1];
		//printf("helper_vertex-&gt;adjacent_number = %d\n",vertex_array[my_vertex1].adjacent_number);
		helper_vertex-&gt;adjacent_triangles[helper_vertex-&gt;adjacent_number] = i;
		(helper_vertex-&gt;adjacent_number)++;

		//Vertex 2
		helper_vertex = &amp;vertex_array[my_vertex2];
		helper_vertex-&gt;adjacent_triangles[helper_vertex-&gt;adjacent_number] = i;
		(helper_vertex-&gt;adjacent_number)++;

		//Vertex 3
		helper_vertex = &amp;vertex_array[my_vertex3];
		helper_vertex-&gt;adjacent_triangles[helper_vertex-&gt;adjacent_number] = i;
		(helper_vertex-&gt;adjacent_number)++;
		//printf("FlÃ¤chennormale berechnen ... \n");
		set_face_normal(my_vertex1,my_vertex2,my_vertex3,i);
		i++;
	}

	set_vertex_normals();
	//print_triangle_list();
	//Mittelpunkt bestimmen, wichtig fÃ¼rs Kugelmapping
	scale = 85/get_center(); //total unlogische Funktionsbenennung=)
	
	//Skalieren, da die Figur sonst viel zu klein ist
	i = 0;
	printf("Skalierung ist %f\n",scale);
	while(i &lt; _nVertices){
		vertex_array[i].x *= scale;
		vertex_array[i].y *= scale;
		vertex_array[i].z *= scale;
		i++;
	}


	//printf("OFF-file gelesen\n");

	fclose(file);
}

void print_triangle_list(){
	int i, j;
	float x,y,z;
	for(i = 0; i &lt; _nFaces; i++){
		printf("Dreieck %d: ",i);
		for(j = 0; j &lt; 3; j++){
			x = vertex_array[triangle_array[i].vertex_index[j]].x;
			y = vertex_array[triangle_array[i].vertex_index[j]].y;
			z = vertex_array[triangle_array[i].vertex_index[j]].z;
			printf("[%f,%f,%f]\n",x,y,z);
		}
	}
}

void flipTextureImage(void)
{
	int x, y;
	unsigned char *copy;
	copy = malloc(3 * _texX * _texY * sizeof(unsigned char));
	
	for(y = 0; y &lt; _texY; y++)
	{
		for(x = 0; x &lt; _texX * 3; x++)
		{
			copy[y * _texX * 3 + x] = _textureImage[(_texY - y) * _texX * 3 + x];
		}
	}

	free(_textureImage);
	_textureImage = copy;
}

void drawOffModel(void)
{
	// XXX - insert code here
	// consider flags _environmentMapping, _showTexture, _doLighting, _showNormals
	
	draw_texture();

	if(_doLighting == ON){
		glEnable(GL_LIGHTING);
	}
	else{
		glDisable(GL_LIGHTING); 
		glColor3f(1.0f,1.0f,1.0f);
	}

	draw_triangles();

	if(_showNormals == ON){
		draw_normals();
	}
}

void draw_triangles(){
	int i = 0;

	glBegin(GL_TRIANGLES);
	while(i &lt; _nFaces){
		if(_environmentMapping == ON){
			draw_env_triangle(i);
		}
		else{
			draw_non_env_triangle(i);
		}
		i++;
	}
	glEnd();
}

void draw_non_env_triangle(int index){
	int index_helper;
	int i;
	for(i = 0; i &lt; 3; i++){
		index_helper = triangle_array[index].vertex_index[i];

		glTexCoord2f(vertex_array[index_helper].u,vertex_array[index_helper].v);
		glNormal3f(vertex_array[index_helper].nx,
		vertex_array[index_helper].ny,
		vertex_array[index_helper].nz);
		glVertex3f(vertex_array[index_helper].x,
		vertex_array[index_helper].y,
		vertex_array[index_helper].z);
	}
}

void draw_env_triangle(int index){

	int index_helper;
	int i;
	for(i = 0; i &lt; 3; i++){
		index_helper = triangle_array[index].vertex_index[i];

		glNormal3f(vertex_array[index_helper].nx,
		vertex_array[index_helper].ny,
		vertex_array[index_helper].nz);
		glVertex3f(vertex_array[index_helper].x,
		vertex_array[index_helper].y,
		vertex_array[index_helper].z);
	}
}

void draw_normals(){
	int i;
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	for(i = 0; i &lt; _nVertices; i++){
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(vertex_array[i].x,vertex_array[i].y,vertex_array[i].z);

		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(vertex_array[i].x + vertex_array[i].nx,vertex_array[i].y + vertex_array[i].ny,vertex_array[i].z + vertex_array[i].nz);
	}
	glEnd();
	//printf("test");
}

//Gibt eine Strahl im Worldview auf den Punkt der Textur wieder, an dem sich der Mauszeiger im Textureview befindet
void drawCursor()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(1.0);
	
	glBegin(GL_LINES);
		glColor3ub(0,0,255);
		//erzeuge eine Gerade vom Ursprung des Weltkoordinatensystems durch den Schnittpunkt des Models an der sich die gemappte Textur befindet
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(SCALE*cursor[0],SCALE*cursor[2],SCALE*cursor[1]); // z und y sind vertauscht
	glEnd();
}

void init_texture(){

	glGenTextures(1,&amp;texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _texX, _texY, 0, GL_RGB, GL_UNSIGNED_BYTE, _textureImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}

void draw_texture(){
	if(_showTexture == ON){
		if (_environmentMapping == OFF){
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
		}
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texName);
		
		if(_environmentMapping == ON){
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glTexGenf(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
			glTexGenf(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
		}
	}
}
/***************************** setup &amp; callback functions *******************************/

void setTexture(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void setModelview(void)
{
	// setup model matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void setProjection(void)
{
	// setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(_fovy, _width / (float) _height, 1.0, 250.0);
}

void setup(void)
{
	// general GL settings
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT_MODEL_LOCAL_VIEWER);
	glShadeModel(GL_SMOOTH);
}

/**************** main window ******************/

void reshape(int width, int height)
{
	// update global variables
	_width = width;
	_height = height;

	// update viewport
	glViewport(0, 0, _width, _height);

	// update projection matrix
	setProjection();
}

void displayMain(void)
{
	glClearColor(0.8, 0.8, 0.8, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
        exit(0);
    }
} 

/***************** texture window ***********************/

void displayTexture(void)
{
	// setup projection matrix
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(0.0f, _width , _height, 0.0f);

	// setup model matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glClearColor(0.5, 0.5, 0.5, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(_textureImage != NULL)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glRasterPos2i(0, _texY);
		glDrawPixels(_texX, _texY, GL_RGB, GL_UNSIGNED_BYTE, _textureImage);
	}

	glutSwapBuffers();
}

void setPointInTexture(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	if((x &lt; 0) || (x &gt;= _texX) || (y &lt; 0) || (y &gt;= _texY))
		return;
	_textureImage[x * 3 + y * _texX * 3] = r;
	_textureImage[x * 3 + 1 + y * _texX * 3] = g;
	_textureImage[x * 3 + 2 + y * _texX * 3] = b;
}

void textureMouse(int button, int state, int x, int y)
{
	// XXX- insert code here
	switch(button){
	case GLUT_LEFT_BUTTON: 
		if(state == GLUT_DOWN){
			setPointInTexture(x , abs(y-(_height - 2)) , 255 , 0 , 0);
			glutPostRedisplay();
			
			glutSetWindow(_worldWindow);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _texX, _texY, GL_RGB, GL_UNSIGNED_BYTE, _textureImage);
			glutPostRedisplay();
			glutSetWindow(_worldWindow);
			updatePointer(x,y);
			glutPostRedisplay();
		}
		break;
	}
	
	//sichere alte Mauskoordinaten
	savedPos[0] = x;
	savedPos[1] = y;
}

void textureMotion(int x, int y)
{
	float m , n;
	int y_tmp , x_start , y_start, x_end, y_end , i;
	
	//Ã¼berprÃ¼fe ob die Linie in negative x bzw y Richtung gezeichnet werden soll, wenn ja drehe Werte
	if (x &lt; savedPos[0]){
	 x_start = x;
	 x_end = savedPos[0];
	}else{
	 x_start = savedPos[0];
	 x_end = x;
	}
	if(y &lt; savedPos[1]){
		y_start = y;
		y_end = savedPos[1];
	}
	else{
		y_start = savedPos[1];
		y_end = y;
	}
	// XXX- insert code here
	//wenn die linke Maustaste gedrÃ¼ckt gehalten wird sollen Linien gezeichnet werden
	
	
	//nur in y-Richtung
	if(x_start == x_end){
		for(i = y_start ; i&lt;=y_end;i++){
			setPointInTexture((int)x,(int)abs(i-(_height -2)),255,0,0);	
		}
	}
	else{
		//parameter fÃ¼r die Geradengleichung berechnen
		m = (y_end - y_start) / (x_end - x_start);
		n = y_start - m * x_start;
		
		//Gerade in Punkte aufteilen
		for(i = x_start ; i &lt;= x_end ; i++){
			y_tmp = m * i + n;
			setPointInTexture((int)i , (int)abs(y_tmp - (_height - 2)) , 255 , 0 , 0);
		}
	}
	//Zeichne Linie auf der Textur
	glutPostRedisplay();
	glutSetWindow(_worldWindow);
	//Ãœbernehme Textur auf das Model im Worldwindow
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _texX, _texY, GL_RGB, GL_UNSIGNED_BYTE, _textureImage);
	glutPostRedisplay();
	glutSetWindow(_worldWindow);
	updatePointer(x,y);
	glutPostRedisplay();

	savedPos[0] = x;
	savedPos[1] = y;
}

void texturePassiveMotion(int x, int y)
{
	// Wenn keine Taste gedrÃ¼ckt wird aktualisiere nur den Strahl auf die Maus
	updatePointer(x,y);
	
	glutSetWindow(_worldWindow);
	glutPostRedisplay();
}

void replaceTexture(char* filename)
{
	if(_textureImage != NULL)
			free(_textureImage);
	_textureImage = glmReadPPM(filename, &amp;_texX, &amp;_texY);
	flipTextureImage();
	glutPostRedisplay();
	glutSetWindow(_worldWindow);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _texX, _texY, 0, GL_RGB, GL_UNSIGNED_BYTE, _textureImage);
	glutPostRedisplay();
}

void textureMenu(int value)
{
	int x, y;

    switch (value) {
    case 'e':
		replaceTexture("data/earthcyl2.ppm");
        break;
	case 'i':
		replaceTexture("data/earth2.ppm");
        break;
	case 'n':
		replaceTexture("data/earthlights.ppm");
        break;
	case 's':
		replaceTexture("data/saturncyl1.ppm");
		break;
	case 'd':
		replaceTexture("data/stpeters.ppm");
		break;
	case 'x':
		replaceTexture("data/supernova.ppm");
		break;
	case 'u':
		replaceTexture("data/uffizi.ppm");
		break;
	case 'c':
		for(y = 0; y &lt; _texY; y++)
		{
			for(x = 0; x &lt; _texX * 3; x++)
			{
				setPointInTexture(x, y, 255, 255, 255);
			}
		}
		glutPostRedisplay();
		glutSetWindow(_worldWindow);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _texX, _texY, GL_RGB, GL_UNSIGNED_BYTE, _textureImage);
		glutPostRedisplay();
		break;
    }
}

/****************** world window *********************/

void displayWorld(void)
{
	GLfloat _directionalVec[] = { 0.0, 0.0, 1.0, 0.0 };

	setTexture();
	setModelview();

	glClearColor(0.2, 0.2, 0.2, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// set Light position
	glLightfv(GL_LIGHT0, GL_POSITION, _directionalVec);

	// shift object
	glTranslatef(_shiftX, _shiftY, _shiftZ);

	// rotate Object
	glRotatef(_rotObjectX, 1.0f, 0.0f, 0.0f);
	glRotatef(_rotObjectY, 0.0f, 1.0f, 0.0f);

	// show coordinate system
	if(_showCoordinates == ON)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
			glColor3ub(255, 0, 0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(5.0, 0.0, 0.0);

			glColor3ub(0, 255, 0);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, 5.0, 0.0);

			glColor3ub(0, 0, 255);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(0.0, 0.0, 5.0);
		glEnd();
	}

	// show center of spherical mapping
	if(_showOrigin == ON)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glColor3f(1.0, 1.0, 0.0);
		glPushMatrix();
		glTranslatef(_origin[0], _origin[1], _origin[2]);
		glutSolidSphere(1.0f, 20, 20);
		glPopMatrix();
	}

	// shade and draw the geometry
    	drawOffModel();
	drawCursor();

	glutSwapBuffers();
}

void worldMouse(int button, int state, int x, int y)
{
	int modifier;

	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN)
		{
			_oldX = x;
			_oldY = y;
			modifier = glutGetModifiers();
			if(modifier &amp; GLUT_ACTIVE_CTRL)
				_drag = SHIFT_XY;
			else if(modifier &amp; GLUT_ACTIVE_SHIFT)
				_drag = SHIFT_Z;
			else if(modifier &amp; GLUT_ACTIVE_ALT)
				_drag = SCALE_VIEW;
			else
				_drag = ROTATE;
		}
		else if(state == GLUT_UP)
		{
			_drag = NO_DRAG;
		}
		break;
	}
}

void worldMotion(int x, int y)
{
	switch(_drag)
	{
	case ROTATE:
		_rotObjectX += (y - _oldY);
		_rotObjectY += (x - _oldX);
		break;
	case SHIFT_XY:
		_shiftX += (x - _oldX) / 10.0;
		_shiftY -= (y - _oldY) / 10.0;
		break;
	case SHIFT_Z:
		_shiftZ += (y - _oldY) / 10.0;
		break;
	case SCALE_VIEW:
		_fovy -= (y - _oldY);
		// restrict the field of view within [1,179]
		if(_fovy &gt; 179.0)
			_fovy = 179.0;
		else if(_fovy &lt; 1.0)
			_fovy = 1.0;
		setProjection();
		break;
	}
	_oldX = x;
	_oldY = y;
	glutPostRedisplay();
}

void worldMenu(int value)
{   
	switch(value)
	{
	case 'l':
		_doLighting = !_doLighting;
		break;
	case 't':
		_showTexture = !_showTexture;
		break;
	case 'n':
		_showNormals = !_showNormals;
		break;
	case 'o':
		_showOrigin = !_showOrigin;
		break;
	case 's':
		_showCoordinates = !_showCoordinates;
		break;
	case 'm':
		_environmentMapping = !_environmentMapping;
		break;
	case 'c':
		if(_model != 'c')
		{
			_model = 'c';
			readOffFile("data/cow.off");
			calculateTextureCoordinates();
		}
		break;
	case 'b':
		if(_model != 'b')
		{
			_model = 'b';
			readOffFile("data/bunny2.off");
			calculateTextureCoordinates();
		}
		break;
	case 'g':
		if(_model != 'g')
		{
			_model = 'g';
			readOffFile("data/sphere_new.off");
			calculateTextureCoordinates();
		}
		break;
	case 'x':
		if(_model != 'x')
		{
			_model = 'x';
			readOffFile("data/tetra.off");
			calculateTextureCoordinates();
		}
		break;
	case 'e':
		_model = 'e';
		break;
	}

    glutPostRedisplay();
}

// main function

int main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(_width + 3 * GAP, _height + 2 * GAP);
    glutInitWindowPosition(50, 50);
    glutInit(&amp;argc, argv);
    _mainWindow = glutCreateWindow("Shade - Assignment cg1_ws03/04");
    glutReshapeFunc(reshape);
    glutDisplayFunc(displayMain);
	glutKeyboardFunc(keyboard);

	_textureWindow = glutCreateSubWindow(_mainWindow, GAP, GAP, _width / 2, _height);
	glutReshapeFunc(reshape);
    glutDisplayFunc(displayTexture);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(textureMouse);
	glutMotionFunc(textureMotion);
	glutPassiveMotionFunc(texturePassiveMotion);
	glutCreateMenu(textureMenu);
	glutAddMenuEntry("Tools:", 0);
	glutAddMenuEntry("Pen", 'p');
	glutAddMenuEntry("Clear", 'c');
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Spherical textures:", 0);
	glutAddMenuEntry("Earth", 'e');
	glutAddMenuEntry("Earth (ice)", 'i');
	glutAddMenuEntry("Earth (night)", 'n');
	glutAddMenuEntry("Saturn", 's');
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Environment textures:", 0);
	glutAddMenuEntry("St Peters dome", 'd');
	glutAddMenuEntry("Uffizium", 'u');
	glutAddMenuEntry("Supernova", 'x');
    glutAttachMenu(GLUT_RIGHT_BUTTON);

	_worldWindow = glutCreateSubWindow(_mainWindow, 2 * GAP + _width / 2, GAP, _width / 2, _height);
	glutReshapeFunc(reshape);
    glutDisplayFunc(displayWorld);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(worldMouse);
	glutMotionFunc(worldMotion);
	glutCreateMenu(worldMenu);
	glutAddMenuEntry("Model:", 0);
	glutAddMenuEntry("Cow", 'c');
	glutAddMenuEntry("Stanford Bunny", 'b');
	glutAddMenuEntry("Sphere", 'g');
	glutAddMenuEntry("Tetraeder", 'x');
	glutAddMenuEntry("None", 'e');
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("Rendering:", 0);
	glutAddMenuEntry("Lighting on/off", 'l');
	glutAddMenuEntry("Texture on/off", 't');
	glutAddMenuEntry("Normals on/off", 'n');
	glutAddMenuEntry("Coordinate System on/off", 's');
	glutAddMenuEntry("Origin on/off", 'o');
	glutAddMenuEntry("Environment mapping on/off", 'm');
    glutAttachMenu(GLUT_RIGHT_BUTTON);

	// print info
	printf("Commands:\n");
	printf("Mouse + left button -&gt; rotate object\n");
	printf("Mouse + ALT + left button -&gt; set field of view\n");
	printf("Mouse + CTRL + left button -&gt; shift-xy object\n");
	printf("Mouse + SHIFT + left button -&gt; shift-z object\n");
	printf("Right button -&gt; menu\n");
	printf("ESC -&gt; exit\n");

	setup();
    
	readOffFile("data/tetra.off");
	calculateTextureCoordinates();
	_textureImage = glmReadPPM("data/earth2.ppm", &amp;_texX, &amp;_texY);
	flipTextureImage();
	init_texture();

	glutSetWindow(_worldWindow);
	
	glutPostRedisplay();

    glutMainLoop();
    
    return 0;
}