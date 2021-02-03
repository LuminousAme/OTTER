#version 410

//mesh data from vert shader
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 4) in vec3 inWorldNormal;

//material data
uniform sampler2D s_Diffuse;

//light data
uniform vec3 u_lightPos;
//ambient
uniform float u_ambStr;
uniform vec3 u_ambCol;
//specular
uniform float u_specStr;
//toon shading
uniform int u_bands; 
float scaleFactor = 1.0 / u_bands;
uniform float u_outlineSize;
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

	//specular highlight
	vec3 lightDir = normalize(u_lightPos - inPos);
	vec3 viewDir  = normalize(u_CamPos - inPos);
	vec3 halfWay =  normalize(lightDir + viewDir);
	float spec = pow(max(dot(inNormal, halfWay), 0.0), 256.0); 
	vec3 specularLighting = u_specStr * spec * vec3(1.0, 1.0, 1.0);

	//attenuation
	float dist = length(u_lightPos - inPos);
	float attenuation = 1.0f / (
		u_attenConst + 
		u_attenLin * dist +
		u_attenQuad * dist * dist);

	//cobmine all base lighting 
	vec3 finalLighting = (ambientLighting + specularLighting) * attenuation;
	//add toon shading effect
	finalLighting = floor(finalLighting * u_bands) * scaleFactor;

	//outline
	float edge = (dot(viewDir, inWorldNormal) < u_outlineSize) ? 0.0 : 1.0;

	//save the result and pass it on
	frag_color = vec4(textureColor.rgb * finalLighting, textureColor.a) * vec4(edge, edge, edge, 1.0);
}