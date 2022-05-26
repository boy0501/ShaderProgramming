#version 450

in vec3 a_Position;

const float PI = 3.141592;
uniform float u_Time;

out vec2 v_TexCoord;

void main()
{

	vec4 newPos = vec4(a_Position,1);

	newPos.x += sin(u_Time);
	newPos.y += cos(u_Time);


	gl_Position = newPos;
	v_TexCoord = a_Position.xy;
	//v_TexCoord = vec2( (a_Position.x + 1.0) / 2.0, (a_Position.y + 1.0)/2.0);
}
