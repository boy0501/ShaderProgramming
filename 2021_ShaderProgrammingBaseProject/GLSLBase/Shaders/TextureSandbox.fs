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
	//0~3분의1 , 3분의1~3분의2 ,3분의 2 
	// TIP : 구간은 fract로, 통일성은 floor로 ! 
	//floor로 0~0 ,1~1, 2~2를 만들고 여기에 fract를 더해준다. 그럼 0~0.9,1~1.9, 2~2.9 인데 여기서 3으로 나누면 원하는구간 get
	newTexCoord.y = ((2-floor(v_TexCoord.y * 3)) + fract(v_TexCoord.y * 3)) / 3;

	returnValue = texture(u_TexSampler,newTexCoord);

	return returnValue;
}

vec4 P9()
{
	vec4 returnValue = vec4(0);
	vec2 newTexCoord = v_TexCoord;
	//세로로 두번 그려야하니 0~1,0~1 fract조져 
	//0~0.5 까지는 뒤집어야함 . floor로 0 , 1 구하고 0.5 빼면 위에는 0.5 아래는 -0.5 곱하기 2 하면 1 ~ -1 아래만 -1 곱해서 반전 해
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
