#version 150

//ins
in vec2 texCoordinates;
in vec4 vPosition;
in vec3 vNormal;

//outs
out vec4 pixel;

// Methods
vec4 blinnPhongReflection(vec4 position, vec3 normal);

//uniform
uniform sampler2D tex;
uniform bool lighting;
uniform bool showTexture;

void main(){
	pixel = vec4(1.f, 1.f, 1.f, 1.f);
	
	if(showTexture == true) {
		pixel = vec4(texture2D(tex, texCoordinates).rgb, 1.f);
	}
	
	if(lighting == true) {
		pixel *= blinnPhongReflection(vPosition, vNormal);
	}
}