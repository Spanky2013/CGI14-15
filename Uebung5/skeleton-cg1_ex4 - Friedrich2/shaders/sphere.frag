#version 150

//ins
in vec2 normalTexCoord;
in vec2 texCoordinates;
in vec4 vPosition;
in vec3 vNormal;
in vec2 pasPos;

//outs
out vec4 pixel;

// Methods
vec4 blinnPhongReflection(vec4 position, vec3 normal);

//uniform
uniform sampler2D tex;
uniform bool lighting;
uniform bool showTexture;
uniform mat4 mirror;
uniform bool moveEnvironment;
uniform float pi;

void main(){
	pixel = vec4(1.f, 1.f, 1.f, 1.f);
	
	vec2 tc = texCoordinates;
	//tc.x = (pi + atan(pasPos.x, pasPos.y)) / (2 * pi); // calculate angle

	if(showTexture == true) {
		
		pixel = vec4(texture2D(tex, tc).rgb, 1.f) ;//+ vec4(texture2D(tex,normalTexCoord).rgb, 1.f)*0.4;		
	}
	
	if(lighting == true) {
		pixel *= blinnPhongReflection(vPosition, vNormal);
	}
}