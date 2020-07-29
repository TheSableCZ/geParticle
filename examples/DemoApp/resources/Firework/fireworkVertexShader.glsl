R"(
#version 430

layout(location=0) in vec3 pos;
layout(location=1) in vec4 color;
layout(location=2) in vec2 size;

out vec4 particlecolor;
out vec2 particlesize;

out Particle
{
	vec3 position;
} particle;

void main(void){
	gl_Position = vec4(pos, 1.f);
	particle.position = pos;

	particlecolor = color;
	particlesize = size;
}
)"