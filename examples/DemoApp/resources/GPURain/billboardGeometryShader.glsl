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
	vec4 position;
	bool visible;
} particle[];

out vec4 color;
out vec2 UV;

const vec2 offsets[4] = {
	vec2(0.f, 0.f), vec2(1.f, 0.f), vec2(0.f, 1.f), vec2(1.f, 1.f) };

const vec2 particlesize = vec2(.1f, 1.f);

void main(void){
	if (particle[0].visible == false)
		return;

	for (int i = 0; i < 4; ++i) {
		vec4 pos = particle[0].position;
		pos.xyz += CameraRight * particlesize.x * (offsets[i].x - 0.5f) + CameraUp * particlesize.y * (offsets[i].y - 0.5f);
		gl_Position = projectionMatrix * viewMatrix * pos;

		color = vec4(0.f, 0.62f, 1.f, .4f);
		UV = offsets[i].xy;

		EmitVertex();
	}
}
)"