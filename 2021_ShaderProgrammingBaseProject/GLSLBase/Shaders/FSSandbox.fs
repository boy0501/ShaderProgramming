#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
const float PI = 3.141592;


uniform vec3 u_Points[10];	//�迭�� �ִ�� �� �� �ִ� ������ �ִٰ� ��.
uniform float u_Time;
vec4 CrossPattern()
{
	vec4 returnValue = vec4(1,1,1,1);
	float XAxis = sin(10*(v_Color.x * 2 * PI) + PI/2);	//PI+2 �ڿ� ���ϴ°� ���迡 ���ٰ��� .
	float YAxis = sin(10*(v_Color.y * 2 * PI) + PI/2);
	float resultColor = max(XAxis,YAxis);	//ĥ���� ���� ���Ϸ��� max�� �����͸� ��󳻸��.
	returnValue = vec4(resultColor);
	
	return returnValue;
}

vec4 DrawCircle()
{
	float dis = distance(v_Color.xy,vec2(0.5,0.5));
	vec4 newColor = vec4(0,0,0,0);

	if(dis  <= 0.5)
	{	
		newColor = vec4(1,1,1,1);
	}else{
	
		newColor = vec4(0,0,0,0);
	}

	return newColor;
}

vec4 DrawCircleLine()
{
	float dis = distance(v_Color.xy,vec2(0.5,0.5));
	vec4 newColor = vec4(0,0,0,0);
	if(0.48<= dis && dis  <= 0.5)
	{	
		newColor = vec4(1,1,1,1);
	}else{
	
		newColor = vec4(0,0,0,0);
	}
	
	return newColor;
}

vec4 DrawMultipleCircles()
{
	float dis = distance(v_Color.xy,vec2(0.5,0.5));	// 0~0.5 ��
	float temp =  sin(dis * 4 * PI);	//�� 10�� �׸��� (���蹮��) 
	//dis  = 0 ���� 0.5���� �����ϴ� ��ǥ 
	// *4�� �ؼ� 0 ~ 2 ���� �����ϵ��� ���� �׷� 0~2PI ������ 1 �ֱ��� sin ��� ������� 
	// sin �ֱ� 1���� �� 1����  10���׸��������� 10 ���ϸ� �� 

	return vec4(temp);
}

vec4 DrawCircles()
{
	vec4 returnColor = vec4(0);
	for(int i  = 0 ; i < 10; ++i)
	{	
		float d = distance(u_Points[i].xy,v_Color.xy);
		float temp = sin (10 * d * 4 * PI -  u_Time * 100);
		if(d < u_Time && u_Time < 0.1)
			returnColor += vec4(temp);
		
	}
	
	
	return returnColor;
}

void main()
{
	

	//procedural ��� �Ѵ뿩, �����׸�Ʈ���� �ǽð����� �����°� 
	//gpu�� �������鼭 �ᵵ �� 

	
	//FragColor = CrossPattern();
	//FragColor = DrawCircle();
	//FragColor = DrawCircleLine();
	FragColor = DrawCircles();
}