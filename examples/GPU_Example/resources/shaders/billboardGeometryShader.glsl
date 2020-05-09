#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

//out vec2 ex_TexCoor;

uniform mat4 MVP;
uniform vec3 CameraUp;
uniform vec3 CameraRight;

//uniform mat4  viewMatrix;
//uniform mat4  projMatrix;
//uniform vec4  camPos;
//uniform float quadLength;

const vec2 offsets[4] = {
	vec2(0.f, 0.f), vec2(1.f, 0.f), vec2(0.f, 1.f), vec2(1.f, 1.f) };

void main(void){
	for (int i = 0; i < 4; ++i) {
		vec4 pos = gl_in[0].gl_Position;
		//pos.xy += .5f * (offsets[i] - vec2(0.5f));
		pos.xyz += CameraRight * .5f * (offsets[i].x - 0.5f) + CameraUp * .5f * (offsets[i].y - 0.5f);
		gl_Position = MVP * pos;
		EmitVertex();
	}
}
