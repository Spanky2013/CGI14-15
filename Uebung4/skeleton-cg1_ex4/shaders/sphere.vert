#version 150
//ins
in vec3 position;
in vec3 normal;
in vec2 texCoord;

//outs
out vec2 texCoordinates;
out vec3 vNormal;
out vec4 vPosition;

//const
const vec3 Xunitvec = vec3 (1.0, 0.0, 0.0);
const vec3 Yunitvec = vec3 (0.0, 1.0, 0.0);

//uniforms
uniform float pi;
uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjection;

void main(){

	vPosition = modelView * vec4(position, 1.);
	vec3 eye = normalize(vec3(vPosition));
	vNormal = normalize((normalMatrix*vec4(normal,0)).xyz);
	
	vec3 refl = reflect(eye,vNormal);

	float divisor = sqrt(
		pow(refl.x,2.f) + 
		pow(refl.y,2.f) + 
		pow(refl.z+1,2.f)
	);

	refl.x = (refl.x/divisor + 1)/2;
	refl.y = (refl.y/divisor + 1)/2;
	//refl.z = refl.z/divisor;

	//float u = 0.5 + (atan(-refl.x,-refl.z))/(2*pi);
	//float v = acos(-refl.y)/(pi);

	texCoordinates = refl.xy;//vec2(u,v);

	gl_Position = modelViewProjection*vec4(position, 1.);
	
	
}