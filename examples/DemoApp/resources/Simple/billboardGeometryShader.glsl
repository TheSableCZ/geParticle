R"(
#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;


uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 CameraUp;
uniform vec3 CameraRight;

in Particle
{
	vec3 position;
} particle[];

in vec4 particlecolor[];
in vec2 particlesize[];

out vec4 color;
out vec2 UV;

const vec2 offsets[4] = {
	vec2(0.f, 0.f), vec2(1.f, 0.f), vec2(0.f, 1.f), vec2(1.f, 1.f) };


void main(void){
	for (int i = 0; i < 4; ++i) {

		vec4 pos = vec4(particle[0].position, 1.f);

		pos.xyz += CameraRight * particlesize[0].x * (offsets[i].x - 0.5f) + CameraUp * particlesize[0].y * (offsets[i].y - 0.5f);
		gl_Position = projectionMatrix * viewMatrix * pos;

		color = particlecolor[0];
		UV = offsets[i].xy;

		EmitVertex();
	}
}
)"