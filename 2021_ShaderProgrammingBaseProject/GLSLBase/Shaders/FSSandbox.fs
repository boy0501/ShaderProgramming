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
	// sin �ֱ� 1���� �� 1����  10���׸�������� 10 ���ϸ� �� 

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

vec4 RadarCircle()
{
	float d = distance(vec2(0.5,0),v_Color.xy);
	float sinValue = sin(d*2*PI - u_Time*100);
	sinValue = clamp(pow(sinValue,4),0,1);
	
	vec4 returnColor = vec4(0.5 * sinValue);

	for(int i = 0 ; i < 10; ++i)
	{
		float dTemp = distance(u_Points[i].xy,v_Color.xy);
		float dret = distance(returnColor.xy,vec2(0,0));
		if(dTemp < 0.1)
		{
			returnColor += 
			vec4(0,10 * sinValue * (0.1 - dTemp),0,0);
		}
	}
	return returnColor;
}

vec4 RadarStick()
{
	
	float t = u_Time;
	if(u_Time > 2)
	{
		t = u_Time / 2;
	}
	vec2 A = vec2(0.5,0.2);
	vec2 B = vec2(0.5,-0.2);
	vec2 D = vec2(-0.5,0.2);
	vec2 C = vec2(-0.5,-0.2);
	float radiantime = t * PI; 
	float cosx = cos(radiantime);
	float sinx = sin(radiantime);
	mat2 rotate = mat2(cosx,-sinx,sinx,cosx);

	
	//�簢�� 4�� ���ϴ� �� 
	//���� ȸ����� ���Ѵ�. 



	vec2 r1 = rotate * A;//vec2(A.x*cosx + A.y * -sinx,A.x*sinx+A.y*cosx);
	vec2 r2 = rotate * B;//vec2(B.x*cosx + B.y * -sinx,B.x*sinx+B.y*cosx);
	vec2 r3 = rotate * C;//vec2(C.x*cosx + C.y * -sinx,C.x*sinx+C.y*cosx);
	vec2 r4 = rotate * D;//vec2(D.x*cosx + D.y * -sinx,D.x*sinx+D.y*cosx);
	r1 += vec2(0.5,0.5);
	r2 += vec2(0.5,0.5);
	r3 += vec2(0.5,0.5);
	r4 += vec2(0.5,0.5);
	vec4 returnColor = vec4(0);

	
	//���� �� �Ǻ� �� 
	vec2 AB =r2-r1;
	vec2 AM =v_Color.xy - r1;
	vec2 BC =r3-r2;
	vec2 BM =v_Color.xy - r2;
	if(	
	((0 <= dot(AB,AM))&& (dot(AB,AM) <= dot(AB,AB))) &&
	((0 <= dot(BC,BM)) && (dot(BC,BM) <= dot(BC,BC)))
	)
	{
		returnColor = vec4(0.4);
	}else{
	
		returnColor = vec4(0);
	}

	for(int i = 0 ; i < 10; ++i)
	{
		float dTemp = distance(u_Points[i].xy,v_Color.xy);
		float temp = sin(dTemp*4*PI);
		float dret = distance(returnColor.xy,vec2(0,0));
		if(dTemp < 0.1 && dret > 0.5)
		{
			returnColor += vec4(dTemp);
		}
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
	//FragColor = DrawCircles();
	FragColor = RadarCircle();
	//FragColor = RadarStick();
}
