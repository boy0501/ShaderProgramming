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
	
	float YValue = XDis * 0.5 * sin((Dis)* 2.0 * PI - u_Time);	//x는 -0,5~0.5 사이의 움직임이라 0.5를 더해서 0~1로바꿈
	float XValue = XDis * 0.5 * sin((Dis)* 2.0 * PI - u_Time);	//x는 -0,5~0.5 사이의 움직임이라 0.5를 더해서 0~1로바꿈
	newPos.x += XValue;
	newPos.y += YValue;


	gl_Position = vec4(newPos, 1);

	v_Color = vec4((sin((Dis)* 2.0 * PI - u_Time) +1 ) /2);
	//v_Color = vec4((sin((a_Position.x + 0.5 )* 2.0 * PI - u_Time) + 1 )/2.0);
}

void surf()
{
	float Dis = distance(a_Position.xy,vec2(0.0,0.0));	
	float XValue = sin(Dis * 2 *  PI - u_Time);
	
	gl_Position = vec4(a_Position, 1);
	

	v_Color = vec4(XValue);
}

void gu()
{
//구는 포기해 그냥 ,,, 
	vec3 newPos = a_Position;
	float Dis = distance(newPos.xy,vec2(0.0,0.0));
	float z = (0.5 - Dis) * 2;
	newPos.z = z;
	float r = sqrt(pow(newPos.x,2) + pow(newPos.y,2));
	float p = sqrt(pow(r,2) + pow(z,2));
	float omega = atan(r/p);
	float theta = atan(newPos.y / newPos.x);
	
	if(newPos.x < 0.0) {
	 theta += PI;
    } else {
        if(newPos.y<0.0) theta += 2* PI;
    }

	if(r < 0.0) {
	 omega += PI;
    } else {
        if(newPos.z<0.0) omega += 2* PI;
    }

	//newPos.x = 0.5 * sin(theta);
	//newPos.y = 0.5 * cos(theta);
	//
	newPos.x = sin(omega) * cos(theta);
	newPos.y = cos(omega) * sin(theta);
	//newPos.z = cos(omega);


	gl_Position = vec4(newPos, 1);	
	v_Color = vec4(z);
}

void si()
{
	
	vec3 newPos = a_Position;
	float XDis = a_Position.x + 0.5;
	float Dis = distance(a_Position.xy,vec2(-0.5,0.0));
	
	float YValue = XDis * 0.5 * sin((Dis)* 2.0 * PI - u_Time);	//x는 -0,5~0.5 사이의 움직임이라 0.5를 더해서 0~1로바꿈
	float XValue = XDis * 0.5 * sin((Dis)* 2.0 * PI - u_Time);	//x는 -0,5~0.5 사이의 움직임이라 0.5를 더해서 0~1로바꿈

	float ZValue = sin()
	newPos.x += XValue;
	newPos.y += YValue;


	gl_Position = vec4(newPos, 1);

	v_Color = vec4((sin((Dis)* 2.0 * PI - u_Time) +1 ) /2);

}

void main()
{
	si();
}
