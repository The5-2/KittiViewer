#version 330
#extension GL_ARB_separate_shader_objects: enable

precision highp float;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;


layout(location = 0) in  vec3 vPosition; 
layout(location = 1) in  vec3 vColor; 

out float vClip;
out vec4 oColor;

void main(){
	oColor = vec4(vColor, 1.0);
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(vPosition, 1.0);
}