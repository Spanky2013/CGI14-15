#version 150

in vec2 texCoord; 

out vec4 pixel;
// uniform vec4 color;
uniform sampler2D texture;
uniform bool showTexture;

void main(void){
	vec4 color = vec4(1.0,1.0,1.0,1.0);
	color = texture(texture, texCoord);	
	pixel = color;
	
	}
