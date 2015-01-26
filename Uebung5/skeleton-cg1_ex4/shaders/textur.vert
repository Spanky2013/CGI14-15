#version 150

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTexcoords;
out vec2 pasPos;

uniform float pi;
uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjection;

void main(){
	vertexPosition = modelView * vec4(position, 1.);
	vertexNormal = normalize((normalMatrix * vec4(normal, 1.f)).xyz);
	vertexTexcoords = texCoord;

	pasPos = position.xz;
	
	//vertexTexcoords.x = (pi + atan(position.x,position.z))/(2*pi);

	if(vertexTexcoords.x > 1){
		vertexTexcoords.x = vertexTexcoords.x-1;
	}
	if(vertexTexcoords.x < 0){
		vertexTexcoords.x = vertexTexcoords.x+1;		
	}
	if(vertexTexcoords.y > 1){
		vertexTexcoords.y = vertexTexcoords.y-1;
	}
	if(vertexTexcoords.y < 0){
		vertexTexcoords.y = vertexTexcoords.y+1;
	}

	
	gl_Position = modelViewProjection * vec4(position, 1.);
}
