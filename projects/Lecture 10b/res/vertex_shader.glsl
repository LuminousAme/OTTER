#version 410
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_uv;

out vec3 color;
out vec2 texUV;

uniform mat4 MVP;
uniform float time;
uniform float speed;

uniform sampler2D myTextureSampler;

void main() {
	
	color = vertex_color;
	texUV = vertex_uv;

	//height map
	vec3 vert = vertex_pos;
	//vert.y = texture(myTextureSampler, vertex_uv).r;

	//sin
	vert.y = 0.2 * sin(vert.x * 4.0 + time * speed);
	
	gl_Position = MVP * vec4(vert, 1.0);
	//gl_Position = MVP * vec4(vertex_pos, 1.0);
}
	