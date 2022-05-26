#version 450

in vec3 a_Position;

const float PI = 3.141592;
uniform float u_Time;

void main()
{
	float newStart = a_Position.x + 1.0;
	vec4 newPos = vec4(a_Position.x, sin((u_Time + PI * a_Position.x) * 2),a_Position.z,1);
	gl_Position = newPos;
}
