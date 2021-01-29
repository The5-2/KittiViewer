#version 330

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform float uGlPointSize;

layout(location = 0) in vec4 vPosition;

out vec3 oPosition;

void main(){
	oPosition = vPosition.xyz;

	gl_PointSize = uGlPointSize;
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(vPosition.xyz, 1.0);
}