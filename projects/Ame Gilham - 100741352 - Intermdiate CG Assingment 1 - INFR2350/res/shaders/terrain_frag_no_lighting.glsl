#version 420

//mesh data from vert shader
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in float inHeight;

//material data
layout(binding=0) uniform sampler2D s_base;
layout(binding=1) uniform sampler2D s_second;
layout(binding=2) uniform sampler2D s_third;

//camera data
uniform vec3  u_CamPos;

//result
out vec4 frag_color;

//function to remap a value from one range to another
float Remap(float inputMin, float inputMax, float outputMin, float outputMax, float value);

void main() {
	//sample the textures
	vec4 textureColor = vec4(1.0, 1.0, 1.0, 1.0);

	//pick the texture
	float t = Remap(0.0, 3.0, 0.0, 1.0, inHeight);
	if(inHeight <= 1.5) {
		textureColor = mix(texture(s_base, inUV), texture(s_second, inUV), smoothstep(0.0, 0.5, t));
	}
	else {
		textureColor = mix(texture(s_second, inUV), texture(s_third, inUV), smoothstep(0.5, 1.0, t));
	}

	//save the result and pass it on
	frag_color = textureColor;
}

//function to remap a value from one range to another
float Remap(float inputMin, float inputMax, float outputMin, float outputMax, float value) {
	float t = (value - inputMin) / (inputMax - inputMin);
	return mix(outputMin, outputMax, t);
}