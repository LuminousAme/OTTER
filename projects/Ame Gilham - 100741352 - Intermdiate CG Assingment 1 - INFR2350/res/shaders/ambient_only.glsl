#version 410

//mesh data from vert shader
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

//material data
uniform sampler2D s_Diffuse;

//light data
uniform vec3 u_lightPos;
//ambient
uniform float u_ambStr;
uniform vec3 u_ambCol;
//attenuation
uniform float u_attenConst;
uniform float u_attenLin;
uniform float u_attenQuad;

//camera data
uniform vec3  u_CamPos;

//result
out vec4 frag_color;

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
	//sample the texture
	vec4 textureColor = texture(s_Diffuse, inUV);

	//ambient lighting
	vec3 ambientLighting = u_ambStr * u_ambCol;

	//attenuation
	float dist = length(u_lightPos - inPos);
	float attenuation = 1.0f / (
		u_attenConst + 
		u_attenLin * dist +
		u_attenQuad * dist * dist);

	ambientLighting = ambientLighting * attenuation;

	//save the result and pass it on
	frag_color = vec4(textureColor.rgb * ambientLighting, textureColor.a);
}