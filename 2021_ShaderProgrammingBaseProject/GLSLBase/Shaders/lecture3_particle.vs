#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
uniform float u_Time;

uniform vec3 u_Accel;

bool bLoop = true;	//����.. �����ϳ� ���ϳ� ���迡 ���� 

void main()
{
	vec3 newPos;
	float t = u_Time - a_EmitTime;
	float tt = t * t;
	if(t > 0 && ( bLoop || t < a_LifeTime ))	//������ �Ϸ��� bLoop�� true�϶� LifeTime �����ϰ� if������ ���������.
	{
		float temp = t / a_LifeTime;
		float fractional = fract(temp);	//�Ҽ��� �Ʒ��� ��������
		t = fractional * a_LifeTime;
		tt = t * t;
		newPos = a_Position + t * a_Velocity + 0.5 * u_Accel * tt;
	}else
	{
	
		newPos = vec3(-1000000,-100000,-1000000);
	}
		gl_Position = vec4(newPos, 1);
}
