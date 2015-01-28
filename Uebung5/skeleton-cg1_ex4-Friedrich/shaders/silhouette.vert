#version 150

in vec3 position;
in vec3 normal;

out vec4 color;

uniform float pi;
uniform mat4 projection;
uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjection;

void main(){


	gl_Position = modelViewProjection*vec4(position, 1.);
	
	vec4 vPosition = modelView * vec4(position, 1.);
	vec3 eye =  normalize(vec3(vPosition));
	vec3 vNormal = normalize((normalMatrix*vec4(normal,0)).xyz);
	vec3 refl = reflect(eye,vNormal);
	vNormal = (projection*vec4(vNormal,1.)).xyz;
	float angle = 0.5 + asin(cross(eye,vNormal)/(length(eye)*length(vNormal))) / pi;
	//float angle2 = 0.5 + asin(cross(position,normal)/(length(position)*length(normal))) / pi;
	
	
	float scalar = vNormal.x*gl_Position.x +
			gl_Position.y*vNormal.x +
			vNormal.z*gl_Position.z;
			
	if(angle < 0.5){
		color = vec4(1.,0.2,0.2,1.);
	}else{
		color = vec4(1.,1.,1.,1.);
	}
	
	
	float divisor = sqrt(
		pow(refl.x,2.f) + 
		pow(refl.y,2.f) + 
		pow(refl.z+1,2.f)
	);

	refl.x = (refl.x/divisor + 1)/2;
	refl.y = (refl.y/divisor + 1)/2;
	
	// x und y sind jetz u und v



}