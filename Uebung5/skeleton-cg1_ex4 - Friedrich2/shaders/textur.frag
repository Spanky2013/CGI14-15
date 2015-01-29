#version 150

in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexcoords;
in vec2 pasPos;

out vec4 pixel;

vec4 blinnPhongReflection(vec4 position, vec3 normal);

uniform float pi;
uniform sampler2D tex;
uniform bool lighting;
uniform bool showTexture;

void main(void){ 
	pixel = vec4(1.f, 1.f, 1.f, 1.f);

	vec2 tc = vertexTexcoords;
	tc.x = (pi + atan(pasPos.x, pasPos.y)) / (2 * pi); // calculate angle

	if(showTexture == true) {
		pixel = texture2D(tex, tc);
	}

	if(lighting == true) {
		pixel *= blinnPhongReflection(vertexPosition, vertexNormal);
	}
}
