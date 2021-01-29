#version 330

layout(location = 0)  out vec4 out0; // color 

in vec2 tc;
uniform sampler2D texture;
uniform int uChannels;
void main() 
{ 

	if(uChannels == 1){
		out0 = vec4(texture2D(texture, tc).rrr, 1);
	}else if(uChannels == 2){
		out0 = vec4(texture2D(texture, tc).rg, 1, 1);
	}else if(uChannels == 3){
		out0 = vec4(texture2D(texture, tc).rgb, 1);
	}else if(uChannels == 4){
		out0 = vec4(texture2D(texture, tc).rgba);
	}else{
		out0 = vec4(texture2D(texture, tc).rgb, 1);
	}
}