#version 410

layout(location = 1) in vec3 inColor;

out vec4 frag_color;

void main() { 
	//average the colours together to get the greyscale value
	float greyVal = inColor.x + inColor.y + inColor.z; 
	greyVal = greyVal / 3;

	//set the calculated greyscale value to all 3 of the RBG values (this produces a grayscale colour)
	frag_color = vec4(greyVal, greyVal, greyVal, 1.0);
}