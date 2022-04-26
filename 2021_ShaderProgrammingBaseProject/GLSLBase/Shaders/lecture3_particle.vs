#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
uniform float u_Time;

uniform vec3 u_Accel;

void main()
{
	vec3 newPos;
	float t = u_Time - a_EmitTime;
	float tt = t * t;
	if(t > 0)
	{
		newPos = a_Position + t * a_Velocity + 0.5 * u_Accel * tt;
		gl_Position = vec4(newPos, 1);
	}else
	{
	
		newPos = vec3(-1000000,-100000,-1000000);
		//gl_Position = vec4(newPos, 1);	이걸 추가해야 EmitTime이 아닐때 안 보임
	}
}
