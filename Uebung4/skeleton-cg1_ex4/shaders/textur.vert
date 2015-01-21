#version 150

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTexcoords;

uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjection;

void main(){
	vertexPosition = modelView * vec4(position, 1.);
	vertexNormal = normalize((normalMatrix * vec4(normal, 1.f)).xyz);
	vertexTexcoords = texCoord;
	gl_Position = modelViewProjection * vec4(position, 1.);
}
