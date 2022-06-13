#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	//default settings
	glClearDepth(1.f);

	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_Lecture3Shader = CompileShaders("./Shaders/lecture3.vs", "./Shaders/lecture3.fs");
	m_Lecture3ParticleShader = CompileShaders("./Shaders/lecture3_particle.vs", "./Shaders/lecture3_particle.fs");
	m_FSSandboxShader = CompileShaders("./Shaders/FSSandbox.vs", "./Shaders/FSSandbox.fs");
	m_LineSegmentShader = CompileShaders("./Shaders/LineSegment.vs", "./Shaders/LineSegment.fs");
	m_LineFullRectShader = CompileShaders("./Shaders/FullRect.vs", "./Shaders/FullRect.fs");
	m_TextureSandboxShader = CompileShaders("./Shaders/TextureSandbox.vs", "./Shaders/TextureSandbox.fs");
	//Create VBOs
	CreateVertexBufferObjects();
	CreateParticle(1000);
	CreateLine(50);
	CreateTextures();
	m_TexRGB = CreatePngTexture("rgb.png");
	//Initialize camera settings
	m_v3Camera_Position = glm::vec3(0.f, 0.f, 1000.f);
	m_v3Camera_Lookat = glm::vec3(0.f, 0.f, 0.f);
	m_v3Camera_Up = glm::vec3(0.f, 1.f, 0.f);
	m_m4View = glm::lookAt(
		m_v3Camera_Position,
		m_v3Camera_Lookat,
		m_v3Camera_Up
	);

	//Initialize projection matrix
	m_m4OrthoProj = glm::ortho(
		-(float)windowSizeX / 2.f, (float)windowSizeX / 2.f,
		-(float)windowSizeY / 2.f, (float)windowSizeY / 2.f,
		0.0001f, 10000.f);
	m_m4PersProj = glm::perspectiveRH(45.f, 1.f, 1.f, 1000.f);

	//Initialize projection-view matrix
	m_m4ProjView = m_m4OrthoProj * m_m4View; //use ortho at this time
	//m_m4ProjView = m_m4PersProj * m_m4View;

	//Initialize model transform matrix :; used for rotating quad normal to parallel to camera direction
	m_m4Model = glm::rotate(glm::mat4(1.0f), glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-0.5, -0.5, 0.f, -0.5, 0.5, 0.f, 0.5, 0.5, 0.f, //Triangle1
		-0.5, -0.5, 0.f,  0.5, 0.5, 0.f, 0.5, -0.5, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float lecture2[]
		=
	{
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0
	};//9 floats array

	glGenBuffers(1, &m_VBOLecture2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture2), &lecture2, GL_STATIC_DRAW);
	//잦은 변경시 Dynamic_draw

	float lecture3[]
		=
	{
		0.0, 0.0, 0.0, 1,0,0,1,
		1.0, 0.0, 0.0, 0,1,0,1,
		1.0, 1.0, 0.0, 0,0,1,1
	};//21 floats array

	glGenBuffers(1, &m_VBOLecture3);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture3), &lecture3, GL_STATIC_DRAW);
	//잦은 변경시 Dynamic_draw

	float particleSize = 0.1f;
	float lecture3_singleParticle[]
		=
	{
		-particleSize, -particleSize, 0.0, 1,1,1,1,
		 particleSize,  particleSize, 0.0, 1,1,1,1,
		-particleSize,  particleSize, 0.0, 1,1,1,1, // triangle 1
		-particleSize, -particleSize, 0.0, 1,1,1,1,
		 particleSize, -particleSize, 0.0, 1,1,1,1,
		 particleSize,  particleSize, 0.0, 1,1,1,1, // triangle 2

	};//21 floats array

	glGenBuffers(1, &m_VBOSingleParticleQuad);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSingleParticleQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture3_singleParticle), &lecture3_singleParticle, GL_STATIC_DRAW);
	//잦은 변경시 Dynamic_draw



	float rectSize = 0.5f;
	float lecture4_rect[]
		=
	{
		-rectSize, -rectSize, 0.0, 1,1,1,1,	//x,y,z r,g,b,a -> AttribPointer
		 rectSize,  rectSize, 0.0, 1,1,1,1,
		-rectSize,  rectSize, 0.0, 1,1,1,1, // triangle 1
		-rectSize, -rectSize, 0.0, 1,1,1,1,
		 rectSize, -rectSize, 0.0, 1,1,1,1,
		 rectSize,  rectSize, 0.0, 1,1,1,1, // triangle 2

	};//21 floats array

	glGenBuffers(1, &m_VBOSandbox);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSandbox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture4_rect), &lecture4_rect, GL_STATIC_DRAW);
	//잦은 변경시 Dynamic_draw


	float lecture4Pac0_Pos[]
		=
	{
		-rectSize, -rectSize, 0.0,
		 rectSize,  rectSize, 0.0,
		-rectSize,  rectSize, 0.0,
		-rectSize, -rectSize, 0.0,
		 rectSize, -rectSize, 0.0,
		 rectSize,  rectSize, 0.0

	};//21 floats array

	float lecture4Pac0_Color[]
		=
	{
		1,1,1,1,	//x,y,z r,g,b,a -> AttribPointer
		1,1,1,1,
		1,1,1,1, // triangle 1
		1,1,1,1,
		1,1,1,1,
		1,1,1,1 // triangle 2

	};//21 floats array

	glGenBuffers(1, &m_VBOPack0_Pos);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOPack0_Pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture4Pac0_Pos), &lecture4Pac0_Pos, GL_STATIC_DRAW);

	glGenBuffers(1, &m_VBOPack0_Color);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOPack0_Color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture4Pac0_Color), &lecture4Pac0_Color, GL_STATIC_DRAW);
	//잦은 변경시 Dynamic_draw

	float lecture4Pac1[]
		=
	{
		-rectSize, -rectSize, 0.0, 
		 rectSize,  rectSize, 0.0, 
		-rectSize,  rectSize, 0.0, 
		-rectSize, -rectSize, 0.0, 
		 rectSize, -rectSize, 0.0, 
		 rectSize,  rectSize, 0.0, 
		 1,1,1,1,	//x,y,z r,g,b,a -> AttribPointer
		 1,1,1,1,
		 1,1,1,1, // triangle 1
		 1,1,1,1,
		 1,1,1,1,
		 1,1,1,1, // triangle 2

	};//21 floats array

	glGenBuffers(1, &m_VBOPack1);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOPack1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture4Pac1), &lecture4Pac1, GL_STATIC_DRAW);
	//잦은 변경시 Dynamic_draw

	rectSize = 0.1f;
	float lecture5_fullRect[]
		=
	{
		-rectSize, -rectSize, 0.0,
		 rectSize,  rectSize, 0.0,
		-rectSize,  rectSize, 0.0,
		-rectSize, -rectSize, 0.0,
		 rectSize, -rectSize, 0.0,
		 rectSize,  rectSize, 0.0

	};//21 floats array
	glGenBuffers(1, &m_VBOLineFullRect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLineFullRect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture5_fullRect), &lecture5_fullRect, GL_STATIC_DRAW);
	//잦은 변경시 Dynamic_draw

	rectSize = 0.5f;
	float lecture6_PosTex[]
		=
	{
		-rectSize, -rectSize, 0.0, 0.f, 0.f,	//왼아래
		 rectSize,  rectSize, 0.0, 1.f, 1.f,	//오른위
		-rectSize,  rectSize, 0.0, 0.f, 1.f,	//왼쪽위
		-rectSize, -rectSize, 0.0, 0.f, 0.f,	//왼아래
		 rectSize, -rectSize, 0.0, 1.f, 0.f,	//오른아래
		 rectSize,  rectSize, 0.0, 1.f, 1.f		//오른위

	};//21 floats array
	glGenBuffers(1, &m_VBOTexSandbox);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexSandbox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture6_PosTex), &lecture6_PosTex, GL_STATIC_DRAW);
	//잦은 변경시 Dynamic_draw

}

void Renderer::CreateParticle(int count)
{
	int floatCount = count * (3 + 3 + 1 + 1 + 1 + 1 + 1 + 4) * 3 * 2;	//(XYZ+velocityXYZ + emit, life, amp, period, value, rgba) * vertex * 2
	float* particleVertices = new float[floatCount];
	int vertexCount = count * 3 * 2;

	int index = 0;
	float particleSize = 0.01f;
	for (int i = 0; i < count; i++)
	{
		float randomValueX = 0.f;
		float randomValueY = 0.f;
		float randomValueZ = 0.f;		
		float randomValueVX = 0.f;
		float randomValueVY = 0.f;
		float randomValueVZ = 0.f;
		float randomEmitTime = 0.f;
		float randomLifeTime = 0.f;
		float randomAmp = 0.f;
		float randomPeriod = 0.f;
		float randomValue = 0.f;
		float randomR, randomG, randomB, randomA = 0.f;

		randomValueX = 0.f;//((float)rand() / (float)RAND_MAX - 0.5f) * 2.f;//-1~1
		randomValueY = 0.f;// ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f;//-1~1
		randomValueZ = 0.f;
		//원랜 normalize도 해야하지만 하지않음. -1~1은 빠르지만 셰이더에서 변경해줄예정
		randomValueVX =  ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f;//-1~1
		randomValueVY = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f;//-1~1
		randomValueVZ = 0.f;

		randomEmitTime = ((float)rand() / (float)RAND_MAX) * 5.f;
		randomLifeTime = ((float)rand() / (float)RAND_MAX) * 2.f;

		randomAmp =  ((float)rand() / (float)RAND_MAX) * 0.4f - 0.2f;
		randomPeriod = ((float)rand() / (float)RAND_MAX) * 2.f;
		randomValue = ((float)rand() / (float)RAND_MAX) * 1.f;
		randomR = ((float)rand() / (float)RAND_MAX) * 1.f;
		randomG = ((float)rand() / (float)RAND_MAX) * 1.f;
		randomB = ((float)rand() / (float)RAND_MAX) * 1.f;
		randomA = 1.f;
		//v0
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++;	//EmitTime
		particleVertices[index] = randomLifeTime;
		index++;	//LifeTime
		particleVertices[index] = randomAmp;
		index++;	//Amp
		particleVertices[index] = randomPeriod;
		index++;	//Period
		particleVertices[index] = randomValue;
		index++;	//Value
		particleVertices[index] = randomR;
		index++;	//Random Color
		particleVertices[index] = randomG;
		index++;	//Random Color
		particleVertices[index] = randomB;
		index++;	//Random Color
		particleVertices[index] = randomA;
		index++;	//Random Color

		//v1
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++;	//EmitTime
		particleVertices[index] = randomLifeTime;
		index++;	//LifeTime
		particleVertices[index] = randomAmp;
		index++;	//Amp
		particleVertices[index] = randomPeriod;
		index++;	//Period
		particleVertices[index] = randomValue;
		index++;	//Value
		particleVertices[index] = randomR;
		index++;	//Random Color
		particleVertices[index] = randomG;
		index++;	//Random Color
		particleVertices[index] = randomB;
		index++;	//Random Color
		particleVertices[index] = randomA;
		index++;	//Random Color

		//v2
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++;	//EmitTime
		particleVertices[index] = randomLifeTime;
		index++;	//LifeTime
		particleVertices[index] = randomAmp;
		index++;	//Amp
		particleVertices[index] = randomPeriod;
		index++;	//Period
		particleVertices[index] = randomValue;
		index++;	//Value
		particleVertices[index] = randomR;
		index++;	//Random Color
		particleVertices[index] = randomG;
		index++;	//Random Color
		particleVertices[index] = randomB;
		index++;	//Random Color
		particleVertices[index] = randomA;
		index++;	//Random Color

		//v3
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++;	//EmitTime
		particleVertices[index] = randomLifeTime;
		index++;	//LifeTime
		particleVertices[index] = randomAmp;
		index++;	//Amp
		particleVertices[index] = randomPeriod;
		index++;	//Period
		particleVertices[index] = randomValue;
		index++;	//Value
		particleVertices[index] = randomR;
		index++;	//Random Color
		particleVertices[index] = randomG;
		index++;	//Random Color
		particleVertices[index] = randomB;
		index++;	//Random Color
		particleVertices[index] = randomA;
		index++;	//Random Color

		//v4
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++;	//EmitTime
		particleVertices[index] = randomLifeTime;
		index++;	//LifeTime
		particleVertices[index] = randomAmp;
		index++;	//Amp
		particleVertices[index] = randomPeriod;
		index++;	//Period
		particleVertices[index] = randomValue;
		index++;	//Value
		particleVertices[index] = randomR;
		index++;	//Random Color
		particleVertices[index] = randomG;
		index++;	//Random Color
		particleVertices[index] = randomB;
		index++;	//Random Color
		particleVertices[index] = randomA;
		index++;	//Random Color

		//v5
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++;	//Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++;	//EmitTime
		particleVertices[index] = randomLifeTime;
		index++;	//LifeTime
		particleVertices[index] = randomAmp;
		index++;	//Amp
		particleVertices[index] = randomPeriod;
		index++;	//Period
		particleVertices[index] = randomValue;
		index++;	//Value
		particleVertices[index] = randomR;
		index++;	//Random Color
		particleVertices[index] = randomG;
		index++;	//Random Color
		particleVertices[index] = randomB;
		index++;	//Random Color
		particleVertices[index] = randomA;
		index++;	//Random Color

	}

	glGenBuffers(1, &m_VBOManyParticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*floatCount, particleVertices, GL_STATIC_DRAW);
	m_VBOManyParticleVertexCount = vertexCount;

	delete[] particleVertices;
}

void Renderer::CreateLine(int SegCount)
{
	int floatCount = SegCount * 3;// x,y,z
	float* lineVertices = new float[floatCount];
	int vertexCount = SegCount;
	int index = 0;
	for (int i = 0; i < SegCount; ++i)
	{
		lineVertices[index] = -1.f + i * 2.f / (SegCount - 1);
		index++;
		lineVertices[index] = 0.f;
		index++;
		lineVertices[index] = 0.f;
		index++;
	}
	glGenBuffers(1, &m_VBOLineSegment);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLineSegment);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, lineVertices, GL_STATIC_DRAW);
	m_VBOLineSegmentCount = vertexCount;
	delete[] lineVertices;
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}
unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::CreateTextures()
{
	static const GLulong checkerboard[] =
	{
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF
	};
	glGenTextures(1, &m_TexChecker);
	glBindTexture(GL_TEXTURE_2D, m_TexChecker);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture2()
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribPosition);
}

float gTime = 0.f;

void Renderer::Lecture3()
{

	GLuint shader = m_Lecture3Shader;
	glUseProgram(shader);


	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (GLvoid*)(sizeof(float)*3));

	int uniformLocTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformLocTime, gTime);
	int uniformLocColor = glGetUniformLocation(shader, "u_Color");
	glUniform4f(uniformLocColor, 1, 1, 1, 1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	gTime -= 0.01f;
	if (gTime < 0.f) gTime = 1.f;
	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture3_Particle()
{


	GLuint shader = m_Lecture3ParticleShader;
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);

	int attriVelocity = glGetAttribLocation(shader, "a_Velocity");
	glEnableVertexAttribArray(attriVelocity);
	glVertexAttribPointer(attriVelocity, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 3));

	int attriEmitTime = glGetAttribLocation(shader, "a_EmitTime");
	glEnableVertexAttribArray(attriEmitTime);
	glVertexAttribPointer(attriEmitTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 6));

	int attriLifeTime = glGetAttribLocation(shader, "a_LifeTime");
	glEnableVertexAttribArray(attriLifeTime);
	glVertexAttribPointer(attriLifeTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 7));

	int attriAmp = glGetAttribLocation(shader, "a_Amp");
	glEnableVertexAttribArray(attriAmp);
	glVertexAttribPointer(attriAmp, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 8));

	int attriPeriod = glGetAttribLocation(shader, "a_Period");
	glEnableVertexAttribArray(attriPeriod);
	glVertexAttribPointer(attriPeriod, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 9));

	int attriRandomValue = glGetAttribLocation(shader, "a_RandomValue");
	glEnableVertexAttribArray(attriRandomValue);
	glVertexAttribPointer(attriRandomValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 10));

	int attriColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attriColor);
	glVertexAttribPointer(attriColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 11));

	int uniformTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformTime, gTime);

	int uniformAccel = glGetUniformLocation(shader, "u_Accel");
	glUniform3f(uniformAccel, 10.f, 0.0, 0.0);

	gTime += 0.01f;
	//if (gTime > 1.f) gTime = 0.f;

	glDrawArrays(GL_TRIANGLES, 0, m_VBOManyParticleVertexCount);
	glDisableVertexAttribArray(attribPosition);

	glDisable(GL_BLEND);
}

void Renderer::Lecture4_FSSandBox()
{
	GLuint shader = m_FSSandboxShader;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSandbox);	//x,y,z ,r,g,b,a
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

//0~1 사이 값을 만들어냄 
float g_points[] = {
	(float)((float)rand() / (float)RAND_MAX),(float)((float)rand() / (float)RAND_MAX) ,0.01f,
	(float)((float)rand() / (float)RAND_MAX),(float)((float)rand() / (float)RAND_MAX) ,0.01f,
	(float)((float)rand() / (float)RAND_MAX),(float)((float)rand() / (float)RAND_MAX) ,0.01f,
	(float)((float)rand() / (float)RAND_MAX),(float)((float)rand() / (float)RAND_MAX) ,0.01f,
	(float)((float)rand() / (float)RAND_MAX),(float)((float)rand() / (float)RAND_MAX) ,0.01f,
	(float)((float)rand() / (float)RAND_MAX),(float)((float)rand() / (float)RAND_MAX) ,0.01f,
	(float)((float)rand() / (float)RAND_MAX),(float)((float)rand() / (float)RAND_MAX) ,0.01f,
	(float)((float)rand() / (float)RAND_MAX),(float)((float)rand() / (float)RAND_MAX) ,0.01f,
	(float)((float)rand() / (float)RAND_MAX),(float)((float)rand() / (float)RAND_MAX) ,0.01f,
	(float)((float)rand() / (float)RAND_MAX),(float)((float)rand() / (float)RAND_MAX) ,0.01f,


};

void Renderer::Lecture4_RainDrop()
{
	GLuint shader = m_FSSandboxShader;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSandbox);	//x,y,z ,r,g,b,a
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	int UniformPoints = glGetUniformLocation(shader, "u_Points");
	glUniform3fv(UniformPoints, 10, g_points);

	int UniformTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(UniformTime, gTime);
	gTime += 0.001;



	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture4_RadarCircle()
{
	GLuint shader = m_FSSandboxShader;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOPack1);	//x,y,z ,r,g,b,a
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)(sizeof(float) * 18));
	/*
	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSandbox);	//x,y,z ,r,g,b,a
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (GLvoid*)(sizeof(float)*3));
	*/
	/*
	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOPack0_Pos);	//x,y,z
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOPack0_Color);	//x,y,z
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	*/

	int UniformPoints = glGetUniformLocation(shader, "u_Points");
	glUniform3fv(UniformPoints, 10, g_points);

	int UniformTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(UniformTime, gTime);
	gTime += 0.001;



	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture5_LineSegment()
{
	GLuint shader = m_LineSegmentShader;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLineSegment);	//x,y,z 
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);


	int UniformTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(UniformTime, gTime);
	gTime += 0.01;


	glDrawArrays(GL_LINE_STRIP, 0, m_VBOLineSegmentCount);
	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture5_FullRect()
{
	GLuint shader = m_LineFullRectShader;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLineFullRect);	//x,y,z 
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);


	int UniformTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(UniformTime, gTime);
	gTime += 0.01;


	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture6_TexSandbox()
{
	GLuint shader = m_TextureSandboxShader;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexSandbox);	//x,y,z 
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

	int attribTex = glGetAttribLocation(shader, "a_TexCoord");
	glEnableVertexAttribArray(attribTex);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexSandbox);	//x,y,z 
	glVertexAttribPointer(attribTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	int uniformTex = glGetUniformLocation(shader, "u_TexSampler");
	glUniform1i(uniformTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TexRGB);

	int uniformTex1 = glGetUniformLocation(shader, "u_TexSampler1");
	glUniform1i(uniformTex1, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_TexChecker);


	int time = glGetUniformLocation(shader, "u_Time");
	glUniform1f(time, gTime);
	gTime += 0.01;

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTex);
}
