#version 410

layout(location = 1) in vec3 inColor;

out vec4 frag_color;

void main() { 
	//average the colours together to get the greyscale value
	float greyVal = inColor.x + inColor.y + inColor.z; 
	greyVal = greyVal / 3;

	//insert the greyscale value in for all 3 R, B, and G values
	frag_color = vec4(greyVal, greyVal, greyVal, 1.0);
}