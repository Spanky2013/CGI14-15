#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec4 color;

void main(void) {

  color= vec4(1,1,1,1);
  vec4 one= gl_in[1].gl_Position - gl_in[0].gl_Position;
  vec4 two= gl_in[2].gl_Position - gl_in[0].gl_Position;
  float x= one.y*two.z - one.z*two.y;
  float y= one.z*two.x - one.x*two.z;
  float z= one.x*two.y - one.y*two.x;
  vec3 n= vec3(x,y,z);
  color= vec4(normalize(n), 1.0);
  //gl_Position= vec4(n,1.0);
  //EmitVertex();
  gl_Position= gl_in[0].gl_Position;
  EmitVertex();
  gl_Position= gl_in[1].gl_Position;
  EmitVertex();
  gl_Position= gl_in[2].gl_Position;
  EmitVertex();
  EndPrimitive();
}
