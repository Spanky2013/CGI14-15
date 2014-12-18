#version 150

uniform vec3 viewpoint;
uniform mat4 modelViewProjectionMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec4 color;

void main(void) {
 
  vec3 one= gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  vec3 two= gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
  vec3 n= normalize(cross(one,two));
  color= vec4(n, 1);
  color.xyz*=0.5;
  color.xyz+=0.5;
  
  
  gl_Position=  gl_in[0].gl_Position;
  EmitVertex();  
  gl_Position=  gl_in[1].gl_Position;
  EmitVertex();
  gl_Position=  gl_in[2].gl_Position;
  EmitVertex();
  
  EndPrimitive();
}
