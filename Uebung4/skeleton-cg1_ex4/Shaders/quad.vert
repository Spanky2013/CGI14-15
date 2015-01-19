#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
out vec2 texCoord;

void main(){
	//float x = position.x / position.w / 2.0 + 0.5;
	//float y = position.y / position.w / 2.0 + 0.5;
	
	gl_Position = modelViewProjectionMatrix*position;
	texCoord = ((position.xy)*(1/position.w)*0.5) + (0.5,0.5);
	//gl_TexCoord[0] = gl_MultiTexCoord0;
}
