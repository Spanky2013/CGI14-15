#version 150

in vec4 position;

out vec4 texCoord;

void main(){

	gl_position = position;
}