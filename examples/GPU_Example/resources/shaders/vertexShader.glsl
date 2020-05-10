#version 430

layout(location=0) in vec4 pos;
layout(location=1) in float life;

out Particle
{
	vec4 position;
	bool visible;
} particle;

void main(void){
	gl_Position = pos;
	particle.position = pos;
	if (life <= 0)
		particle.visible = false;
	else particle.visible = true;
}