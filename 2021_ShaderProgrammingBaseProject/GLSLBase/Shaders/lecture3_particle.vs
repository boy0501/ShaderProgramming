#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
in float a_Amp;
in float a_Period;
uniform float u_Time;

uniform vec3 u_Accel;

bool bLoop = true;	//숙제.. 루프하냐 안하냐 시험에 나옴 

float g_PI = 3.14;
void main()
{
	vec3 newPos;
	float t = u_Time - a_EmitTime;
	float tt = t * t;
	if(t > 0 && ( bLoop || t < a_LifeTime ))	//루프를 하려면 bLoop가 true일때 LifeTime 무시하고 if문으로 들어와줘야함.
	{
		float temp = t / a_LifeTime;
		float fractional = fract(temp);	//소수점 아래만 가져와줌
		t = fractional * a_LifeTime;
		tt = t * t;

		float period = a_Period;
		float amp = a_Amp;
		newPos.x = a_Position.x + t * a_Velocity.x + 0.5 * u_Accel.x * tt;
		newPos.y = a_Position.y + amp * sin(period * t * 2.0 * g_PI);
		newPos.z = 0;
	}else
	{
	
		newPos = vec3(-1000000,-100000,-1000000);
	}
		gl_Position = vec4(newPos, 1);
}
