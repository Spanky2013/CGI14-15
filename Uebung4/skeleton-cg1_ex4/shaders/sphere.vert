#version 150
//ins
in vec4 position;
in vec4 normal;

//outs
out vec2 texCoord;
out vec3 vNormal;
out vec4 vPosition;

//uniforms
uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjection;

void main(){

	vPosition = modelView * position;
	vNormal = normalize((normalMatrix*normal).xyz);
	
	vec3 refl = reflect(vPosition.xyz,vNormal);
	
	float divisor = 2.f * sqrt(
		pow(refl.x,2.f) + 
		pow(refl.y,2.f) + 
		pow(refl.z+1,2.f)
	);
	
	texCoord = refl.xy/divisor + 0.5;
	
	gl_Position = modelViewProjection*position;
	
	
}