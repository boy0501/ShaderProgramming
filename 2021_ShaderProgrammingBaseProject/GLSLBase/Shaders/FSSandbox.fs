#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;

const float PI = 3.141592;

vec4 CrossPattern()
{
	vec4 returnValue = vec4(1,1,1,1);
	float XAxis = sin(10*(v_Color.x * 2 * PI) + PI/2);	//PI+2 뒤에 더하는거 시험에 낸다고함 .
	float YAxis = sin(10*(v_Color.y * 2 * PI) + PI/2);
	float resultColor = max(XAxis,YAxis);	//칠해진 곳만 구하려면 max로 높은것만 골라내면됨.
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
	float dis = distance(v_Color.xy,vec2(0.5,0.5));	// 0~0.5 임
	float temp =  sin(dis * 4 * PI);	//원 10개 그리기 (시험문제) 
	//dis  = 0 부터 0.5까지 증가하는 좌표 
	// *4를 해서 0 ~ 2 까지 증가하도록 만듬 그럼 0~2PI 까지의 1 주기의 sin 곡선이 만들어짐 
	// sin 주기 1개당 원 1개임  10개그리고싶으면 10 곱하면 됨 

	return vec4(temp);
}

void main()
{
	

	//procedural 라고 한대여, 프래그먼트에서 실시간으로 만들어내는거 
	//gpu가 좋아지면서 써도 됨 

	
	//FragColor = CrossPattern();
	//FragColor = DrawCircle();
	//FragColor = DrawCircleLine();
	FragColor = DrawMultipleCircles();
}
