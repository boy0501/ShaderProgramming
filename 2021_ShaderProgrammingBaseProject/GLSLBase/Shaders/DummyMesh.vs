#version 450

in vec3 a_Position;

const float PI = 3.141592;

uniform float u_Time;
out vec4 v_Color;

void flag()
{
	vec3 newPos = a_Position;
	float XDis = a_Position.x + 0.5;
	float Dis = distance(a_Position.xy,vec2(-0.5,0.0));

	float YValue = XDis * 0.5 * sin((Dis)* 2.0 * PI - u_Time);	//x�� -0,5~0.5 ������ �������̶� 0.5�� ���ؼ� 0~1�ιٲ�
	float XValue = XDis * 0.5 * sin((Dis)* 2.0 * PI - u_Time);	//x�� -0,5~0.5 ������ �������̶� 0.5�� ���ؼ� 0~1�ιٲ�
	newPos.x += XValue;
	newPos.y += YValue;
	gl_Position = vec4(newPos, 1);
	

	v_Color = vec4((sin((a_Position.x + 0.5 )* 2.0 * PI - u_Time) + 1 )/2.0);
}
void main()
{
	flag();
}
