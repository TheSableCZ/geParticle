#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 center;
layout(location = 2) in vec3 color;

out vec3 particlecolor;

//uniform mat4 projection;

uniform mat4 MVP;
uniform vec3 CameraUp;
uniform vec3 CameraRight;

void main()
{
    //gl_Position = vec4(center, 1.0f);

	//projection;

	vec3 position = center + CameraUp * position.y * 0.1 + CameraRight * position.x * 0.1;

	gl_Position = MVP * vec4(position, 1.0f);

	particlecolor = color;

	/*if(gl_VertexID == 0)
   {
      gl_Position = vec4(-0.5, 0.5, 0, 1);
   }
   else if(gl_VertexID == 1)
   {
      gl_Position = vec4(0.5, 0.5, 0, 1);
   }
   else
   {
      gl_Position = vec4(0.5, -0.5, 0, 1);
   }*/
}
