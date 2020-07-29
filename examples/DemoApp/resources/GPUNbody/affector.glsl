R"(
#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

#define G 2.0

// particleCount must be divisible by local_size_x
layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

struct MassPointData {
	vec4 position;
	vec4 velocity;
};

layout(std430, binding=0) buffer Pos {
   MassPointData p[];
};

//uniform vec4 attPos;
uniform float dt;
uniform uint particleCount;

shared vec4 cache[gl_WorkGroupSize.x];

vec3 interaction(in vec4 p1,in vec4 p2)
{
  vec3 r = p2.xyz - p1.xyz;
  float dist_sqr = dot(r,r) + 0.2;
  float dist_sixth = dist_sqr*dist_sqr*dist_sqr;
  float inv_dist_cube = inversesqrt(dist_sixth);
  return r*inv_dist_cube;
}

void main(){
    uint id = gl_GlobalInvocationID.x;

    if(id < particleCount) {
		//p[id].velocity.xyz += vec3(0.0f, -9.81f, 0.0f) * dt;

		vec4 p1 = p[id].position;
		vec3 vel = vec3(0.f);
		for (uint i=0; i<gl_NumWorkGroups.x*gl_WorkGroupSize.x; i+=gl_WorkGroupSize.x)
		{
			vec4 p = p[i+gl_LocalInvocationIndex].position;
			cache[gl_LocalInvocationIndex] = vec4(p.xyz, 1.f);
			groupMemoryBarrier();
			barrier();
			for (int j=0; j < gl_WorkGroupSize.x; ++j)
			{
				vel += interaction(p1, cache[j]);
			}
		}
		vec3 final_vel = dt*vel*G+p[id].velocity.xyz;
		p[id].velocity = vec4(final_vel, 0.f);
		//p[id].velocity = vec4(interaction(p1, cache[1]), 0.f);

		p[id].position.xyz += p[id].velocity.xyz * dt;
    }
}
)"