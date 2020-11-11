#version 410

layout(location = 0) in vec3 inPos;

layout(location = 0) out vec3 outNormal;

uniform mat4 u_SkyboxMatrix;
uniform mat3 u_EnviromentRotation;

void main() {
 vec4 pos = u_SkyboxMatrix * vec4(inPos, 1.0);
 gl_Position = pos.xyww;

 outNormal = u_EnviromentRotation * inPos;
}
