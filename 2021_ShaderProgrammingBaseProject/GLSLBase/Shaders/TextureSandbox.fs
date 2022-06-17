#version 450

layout(location=0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_TexSampler;
uniform sampler2D u_TexSampler1;

uniform float u_Time;
vec4 Flip()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord = vec2(v_TexCoord.x, 1.0 - v_TexCoord.y);
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P1()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.y = abs(2.0*(v_TexCoord.y - 0.5));
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P2()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.x = fract(v_TexCoord.x * 3);
	newTexCoord.y = newTexCoord.y/3.0 + floor(v_TexCoord.x*3.0)/3.0;
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P3()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.x = fract(v_TexCoord.x * 2) + floor(v_TexCoord.y * 2) * 0.5;
	newTexCoord.y = fract(v_TexCoord.y * 2);
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P4()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.x = fract(v_TexCoord.x * 2) ;
	if(v_TexCoord.x > 0.5)
	{
		returnValue = texture(u_TexSampler1,newTexCoord);
	}else{
		returnValue = texture(u_TexSampler,newTexCoord);
	}

	return returnValue;
}
vec4 P5()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	vec2 newTexCoord1 = v_TexCoord;
	newTexCoord1.x += u_Time;
	newTexCoord1.x = fract(newTexCoord1.x);
	returnValue = texture(u_TexSampler,newTexCoord) * texture(u_TexSampler1,newTexCoord1);

	return returnValue;
}

vec4 P6()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;

	float v = floor(v_TexCoord.x *2);
	newTexCoord.x = fract(v_TexCoord.x * 2) ;
	newTexCoord.y = fract(v_TexCoord.y *2)+ v * 0.5;


	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P7()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	
	newTexCoord.x = fract(v_TexCoord.x * 4);
 	newTexCoord.y = newTexCoord.y/3.0 + fract((floor(v_TexCoord.x*4.0))/3.0);
	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P8()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	//0~3����1 , 3����1~3����2 ,3���� 2 
	// TIP : ������ fract��, ���ϼ��� floor�� ! 
	//floor�� 0~0 ,1~1, 2~2�� ����� ���⿡ fract�� �����ش�. �׷� 0~0.9,1~1.9, 2~2.9 �ε� ���⼭ 3���� ������ ���ϴ±��� get
	newTexCoord.y = ((2-floor(v_TexCoord.y * 3)) + fract(v_TexCoord.y * 3)) / 3;

	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P9()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	//���η� �ι� �׷����ϴ� 0~1,0~1 fract���� 
	//0~0.5 ������ ��������� . floor�� 0 , 1 ���ϰ� 0.5 ���� ������ 0.5 �Ʒ��� -0.5 ���ϱ� 2 �ϸ� 1 ~ -1 �Ʒ��� -1 ���ؼ� ���� ��
	float v = (floor(v_TexCoord.y * 2) - 0.5) * 2;
	newTexCoord.y = fract(v_TexCoord.y * 2)  * v;

	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P10()
{
	
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	newTexCoord.x = fract(v_TexCoord.x * 6) + floor(v_TexCoord.y *6) * 0.5 ;
	newTexCoord.y = fract(v_TexCoord.y * 6) ;
	
	returnValue = texture(u_TexSampler,newTexCoord);
	return returnValue;

}
void main()
{
	FragColor = P10();
}
