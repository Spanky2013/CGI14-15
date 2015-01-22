#version 150
//ins
in vec3 position;
in vec3 normal;
in vec2 texCoord;

//outs
out vec2 normalTexCoord;
out vec2 texCoordinates;
out vec3 vNormal;
out vec4 vPosition;
out vec2 pasPos;
//const
const vec3 Xunitvec = vec3 (1.0, 0.0, 0.0);
const vec3 Yunitvec = vec3 (0.0, 1.0, 0.0);

//uniforms
uniform float pi;
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform mat4 mirrorView;
uniform mat4 mirrorNormalMatrix;
uniform mat4 modelViewProjection;
uniform mat4 mirrorViewProjection;
uniform mat4 mirror;
uniform bool moveEnvironment;

void main(){
	//gl_Position = mirrorViewProjection*vec4(position, 1.);
	normalTexCoord = texCoord;
	vec4 p;
		
	vec3 refl;

	if(moveEnvironment){
		vec3 help = position;
		help.x = help.x;
		gl_Position = projection*view*vec4(help,1.);
		p = mirrorView*vec4(position, 1.);
		//p = mirror*vec4(position, 1.);
		pasPos = p.xz;
		vPosition = p;
		vec3 eye =  normalize(vec3(vPosition));
		vNormal = normalize((mirrorNormalMatrix*vec4(normal,0)).xyz);
	    //vNormal = normalize((mirror*vec4(normal,0)).xyz);

		refl = reflect(eye,vNormal);	

		vNormal = normalize((normalMatrix*vec4(normal,0)).xyz);
		vPosition = modelView * vec4(position, 1.);
	}
	else{
		gl_Position = modelViewProjection*vec4(position, 1.);
		p = vec4(position, 1.);
		pasPos = p.xz;
		vPosition = modelView * p;
		vec3 eye =  normalize(vec3(vPosition));
		vNormal = normalize((normalMatrix*vec4(normal,0)).xyz);
	
		refl = reflect(eye,vNormal);	
	}

	float divisor = sqrt(
		pow(refl.x,2.f) + 
		pow(refl.y,2.f) + 
		pow(refl.z+1,2.f)
	);

	refl.x = (refl.x/divisor + 1)/2;
	refl.y = (refl.y/divisor + 1)/2;
	
	// x und y sind jetz u und v
	
	if(refl.x > 1){
		refl.x = refl.x-1;
	}
	if(refl.x < 0){
		refl.x = refl.x+1;		
	}
	if(refl.y > 1){
		refl.y = refl.y-1;
	}
	if(refl.y < 0){
		refl.y = refl.y+1;
	}

	texCoordinates = refl.xy;

	
	
	
}