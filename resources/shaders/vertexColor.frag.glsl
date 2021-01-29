#version 330
  
precision highp float;

layout(location = 0)  out vec4 out0; // color 

in vec4 oColor;

void main() { 
	out0 = oColor;
}