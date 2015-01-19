#version 150

in vec4 coord; 

out vec4 pixel;
uniform vec4 color;

void main(void){ 
 
    pixel= color;		
}
