#version 410

//mesh data from vert shader
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

//material data
uniform sampler2D s_Diffuse;

//camera data
uniform vec3  u_CamPos;

//result
out vec4 frag_color;

void main() {
	//sample the texture
	vec4 textureColor = texture(s_Diffuse, inUV);

	//save the result and pass it on
	frag_color = textureColor;
}