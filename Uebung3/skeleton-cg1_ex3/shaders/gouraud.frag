#version 150

//in vec4 vertexPosition;
//in vec3 vertexNormal;
in vec4 vertexColor;

out vec4 pixel;

vec4 blinnPhongReflection(vec4 position, vec3 normal);

void main(void){ 
	//pixel = blinnPhongReflection(vertexPosition, vertexNormal);
	pixel = vertexColor;
}
