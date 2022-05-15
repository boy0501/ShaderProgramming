#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
in float a_Amp;
in float a_Period;
in float a_RandomValue;
in vec4 a_Color;

out vec4 v_Color;
out vec2 v_TexCoord;
uniform float u_Time;

uniform vec3 u_Accel;

bool bLoop = true;	//숙제.. 루프하냐 안하냐 시험에 나옴 

const float g_PI = 3.14;
const mat3 g_RotMat = mat3(0, -1, 0, 1, 0, 0 ,0 ,0 ,0); 
const vec3 g_Gravity = vec3(0, -0.5, 0);

void main()
{
	vec3 newPos;
	float t = u_Time - a_EmitTime;
	float tt = t * t;
	if(t > 0 && ( bLoop || t < a_LifeTime ))	//루프를 하려면 bLoop가 true일때 LifeTime 무시하고 if문으로 들어와줘야함.
	{
		vec3 newAccel = g_Gravity + a_Velocity;
		newPos.x = sin(a_RandomValue * 2 * g_PI);
		newPos.y = cos(a_RandomValue * 2 * g_PI);
		newPos.z = 0;
		newPos = a_Position + newPos;
		float temp = t / a_LifeTime;
		float fractional = fract(temp);	//소수점 아래만 가져와줌
		t = fractional * a_LifeTime;
		tt = t * t;

		float period = a_Period;
		float amp = a_Amp; 
		newPos = newPos + t * a_Velocity + 0.5 * newAccel * tt;
		vec3 rotVec = normalize(a_Velocity * g_RotMat);
		newPos = newPos + 0.1 * amp * rotVec * sin(period * t * 2.0 * g_PI);
		v_Color = a_Color * (1.0 - fractional);
	}else
	{
	
		newPos = vec3(-1000000,-100000,-1000000);
		v_Color = vec4(0,0,0,0);
	}
	gl_Position = vec4(newPos, 1);
	v_TexCoord = a_Position.xy;
}
