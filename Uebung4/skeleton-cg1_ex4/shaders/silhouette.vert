#version 150

in vec3 position;
in vec3 normal;

out vec4 color;

uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjection;

void main(){


	gl_Position = modelViewProjection*vec4(position, 1.);
	
	vPosition = modelView * vec4(position, 1.);
	vec3 eye =  normalize(vec3(vPosition));
	vNormal = normalize((normalMatrix*vec4(normal,0)).xyz);
	
	vec3 refl = reflect(eye,vNormal);
	
	float divisor = sqrt(
		pow(refl.x,2.f) + 
		pow(refl.y,2.f) + 
		pow(refl.z+1,2.f)
	);

	refl.x = (refl.x/divisor + 1)/2;
	refl.y = (refl.y/divisor + 1)/2;
	
	// x und y sind jetz u und v



}