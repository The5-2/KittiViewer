#version 330
  
precision highp float;

layout(location = 0)  out vec4 out0; // color 

in float vClip;
in vec4 oColor;

void main() { 
	if(vClip > 0.0001) discard;
	out0 = oColor;
}