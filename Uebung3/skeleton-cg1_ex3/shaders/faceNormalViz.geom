#version 150

uniform vec3 viewpoint;
uniform mat4 modelViewProjectionMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 vertexPosition[3];
out vec4 color;

void main(void) {
 
  vec3 edgeV0V1= vec3(vertexPosition[1] - vertexPosition[0]);
  vec3 edgeV0V2= vec3(vertexPosition[2] - vertexPosition[0]);
  vec3 faceNormal= cross(edgeV0V1, edgeV0V2);
  color= vec4(normalize(faceNormal), 1);
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
