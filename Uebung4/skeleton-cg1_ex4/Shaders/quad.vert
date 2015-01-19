#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
out vec4 coord;

void main(){
	GLfloat x = position.x / position.w / 2.0f + 0.5f;
	GLfloat y = position.y / position.w / 2.0f + 0.5f;
	
	gl_Position = modelViewProjectionMatrix*position;
	coord = vec4(x,y,0,1);
}
