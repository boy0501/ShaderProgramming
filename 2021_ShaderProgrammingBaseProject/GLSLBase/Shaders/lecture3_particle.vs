#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;//보여지기 시작 할 시간
in float a_LifeTime;//총 보여질 시간
in float a_Amp;		//진폭
in float a_Period;	//주기
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
		t = fractional * a_LifeTime;	// 총 시간으로 나눈 다음 소숫점만 때 오고 다시 총 시간을 곱해줌으로 인해서 0~LifeTime 주기를 
		//반복할 수 있게 된다. 
		tt = t * t;

		float period = a_Period;
		float amp = a_Amp; 
		newPos = newPos + t * a_Velocity + 0.5 * newAccel * tt;
		vec3 rotVec = normalize(a_Velocity * g_RotMat);	//RotMat을 곱하는것은.속도벡터
		//(어딘가로 향하려 하는 벡터)에다가 90도 회전행렬을 곱해서 나온 벡터를 기준으로
		//위아래로 움직여야 정확하게 사인곡선을 그리기 때문에 구해주는 것 이다. 
		//대각 부스터에 사용하기 위해 쓴 변수임.
		// 벡터에 float의 sin값을 곱해서 벡터방향의로 진자운동하게끔 하는 것 .
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
