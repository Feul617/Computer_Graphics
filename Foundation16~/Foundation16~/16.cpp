#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "file.h"
#include <random>
#include <windows.h>
#include <stdio.h>
#include <string.h>
//#include <gl/glm/glm.hpp>
//#include <gl/glm/ext.hpp>
//#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

#define WIDTH 800
#define HEIGHT 600

#define PI 3.141592

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(0, 1);

class object_state
{
private:
	vec3 trans;
	vec3 rotate;
	vec3 pivot;

public:
	object_state()
	{
		trans.x = 0.0; trans.y = 0.0; trans.z = 0.0;
		rotate.x = 0.0; rotate.y = 0.0; rotate.z = 0.0;
		pivot.x = 0.0; pivot.y = 0.0; pivot.z = 0.0;
	}

	void PlusTrans(GLfloat x, GLfloat y, GLfloat z)
	{
		trans.x += x; trans.y += y; trans.z += z;
	}
	void SetTrans(GLfloat x, GLfloat y, GLfloat z)
	{
		trans.x = x; trans.y = y; trans.z = z;
	}
	void PlusRotate(GLfloat x, GLfloat y, GLfloat z)
	{
		rotate.x += x; rotate.y += y; rotate.z += z;
	}
	void SetRotate(GLfloat x, GLfloat y, GLfloat z)
	{
		rotate.x = x; rotate.y = y; rotate.z = z;
	}
	void SetPibot(GLfloat x, GLfloat y, GLfloat z)
	{
		pivot.x = x; pivot.y = y; pivot.z = z;
	}

	//---멤버변수 GET
	glm::vec3 GetTrans() { return trans; }
	glm::vec3 GetRotate() { return rotate; }
	glm::vec3 GetPibot() { return pivot; }

	glm::mat4 RotateAtPibot(glm::mat4 TR)
	{
		TR = glm::translate(TR, glm::vec3(pivot.x, pivot.y, pivot.z));
		TR = glm::rotate(TR, (GLfloat)glm::radians(rotate.x), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::rotate(TR, (GLfloat)glm::radians(rotate.y), glm::vec3(0.0, 1.0, 0.0));
		TR = glm::rotate(TR, (GLfloat)glm::radians(rotate.z), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::translate(TR, glm::vec3(-1 * pivot.x, -1 * pivot.y, -1 * pivot.z));

		return TR;
	}
	glm::mat4 Translate(glm::mat4 TR)
	{
		TR = glm::translate(TR, glm::vec3(trans.x, trans.y, trans.z));

		return TR;
	}
};

typedef struct FACE {
	int a, b, c;
};

typedef struct READ {
	int vertexNum;
	int faceNum;
	vec3* vertex;
	FACE* face;
};

//변수
GLchar* vertexsource[2], * fragmentsource[2];
GLuint vertexshader[2], fragmentshader[2], vao[3], vbo[3], ebo[2];
GLuint s_program, s_program_plat, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

READ cube;
READ pyramid;

GLfloat middle_line[6][3] = {};
object_state cube_side[6];
object_state pyramid_side[5];

bool ortho_on = false;
bool pyramid_on = false;

//---자전 회전 
GLfloat y_radian = 0;

//함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShader();
void make_fragmentShader();
void InitBuffer();
void InitShader();

void ReadObj(FILE* objFile, READ& Read);

//그리기 함수
void draw_middle_line();

void Keyboard(unsigned char key, int x, int y);
void Timer(int value);


void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WIDTH, HEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	else
		std::cout << "GLEW Initialized\n";

	draw_middle_line();

	cube_side[2].SetPibot(0, 0.5f, 0);
	cube_side[5].SetPibot(0, -1 * 0.5f, 0.5f);

	pyramid_side[0].SetPibot(0, 0, -0.3);
	pyramid_side[1].SetPibot(-0.3, 0, 0);
	pyramid_side[2].SetPibot(0, 0, 0.3);
	pyramid_side[3].SetPibot(0.3, 0, 0);

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");
	int vColorLocation2 = glGetUniformLocation(s_program, "out_Color");
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//---카메라 설정
	vec3 cameraPos = glm::vec3(-0.5f, 0.5f, 1.0f);		 //--- 카메라 위치
	vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);		 //--- 카메라 위쪽 방향
	mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLoc_shape = glGetUniformLocation(s_program, "view"); //--- 뷰잉 변환 설정
	unsigned int viewLoc_line = glGetUniformLocation(s_program, "view"); //--- 뷰잉 변환 설정

	//---투영변환
	glm::mat4 projection = glm::mat4(1.0f);
	if (ortho_on)
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
	else
	{
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0)); //--- 공간을 약간 뒤로 미뤄줌
	}

	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(s_program);

	unsigned int projLoc_shape = glGetUniformLocation(s_program, "projection");
	unsigned int projLoc_line = glGetUniformLocation(s_program, "projection");

	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao[0]);
	glEnableVertexAttribArray(0);

	mat4 xyz = mat4(1.0f);
	xyz = rotate(xyz, radians(30.f), vec3(1.f, 0, 0));
	xyz = rotate(xyz, radians(30.f), vec3(0, -1.f, 0));

	int xyzLocation = glGetUniformLocation(s_program, "modelTransform");
	//--- modelTransform 변수에 변환 값 적용하기
	glUniformMatrix4fv(xyzLocation, 1, GL_FALSE, value_ptr(xyz));

	//중앙선
	glUniform3f(vColorLocation2, 0, 0, 0);
	glDrawArrays(GL_LINES, 0, 6);

	//도형 그리기
	glEnable(GL_DEPTH_TEST);
	glUniformMatrix4fv(viewLoc_shape, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc_shape, 1, GL_FALSE, &projection[0][0]);

	unsigned int modelLoc = glGetUniformLocation(s_program, "model");

	glm::mat4 TR_cube = glm::mat4(1.0f);
	glm::mat4 T_cube = glm::mat4(1.0f);
	glm::mat4 Rx_cube = glm::mat4(1.0f);
	glm::mat4 Ry_cube = glm::mat4(1.0f);
	glm::mat4 S_cube = glm::mat4(1.0f);
	glm::mat4 OS_cube = glm::mat4(1.0f);

	T_cube = glm::translate(T_cube, glm::vec3(0, 0, 0));
	Rx_cube = glm::rotate(Rx_cube, (GLfloat)glm::radians(0.0), glm::vec3(1.0, 0.0, 0.0));
	Ry_cube = glm::rotate(Ry_cube, (GLfloat)glm::radians(y_radian), glm::vec3(0.0, 1.0, 0.0));
	S_cube = glm::scale(S_cube, glm::vec3(1.0, 1.0, 1.0));
	if (pyramid_on == false)
	{
		glBindVertexArray(vao[1]);
		for (int i = 0; i < 6; i++)
		{
			TR_cube = T_cube * Rx_cube * Ry_cube * S_cube;
			TR_cube = cube_side[i].Translate(TR_cube);
			TR_cube = cube_side[i].RotateAtPibot(TR_cube);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TR_cube));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * i * 6));
		}
	}
	else
	{
		glBindVertexArray(vao[2]);
		TR_cube = T_cube * Rx_cube * Ry_cube * S_cube;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TR_cube));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * 0));
		for (int i = 2; i < 6; i++)
		{
			TR_cube = T_cube * Rx_cube * Ry_cube * S_cube;
			//TR_cube = cubeside[i].Translate(TR_cube);
			TR_cube = pyramid_side[i - 2].RotateAtPibot(TR_cube);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TR_cube));
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * i * 3));
		}

	}


	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
{
	glViewport(0, 0, w, h);
}

void make_vertexShader()
{
	vertexsource[0] = filetobuf("vertex.glsl");
	vertexsource[1] = filetobuf("vertex_2d.glsl");

	for (int i = 0; i < 2; i++)
	{
		//--- 버텍스 세이더 객체 만들기
		vertexshader[i] = glCreateShader(GL_VERTEX_SHADER);

		//--- 세이더 코드를 세이더 객체에 넣기
		glShaderSource(vertexshader[i], 1, (const GLchar**)&vertexsource[i], 0);

		//--- 버텍스 세이더 컴파일하기
		glCompileShader(vertexshader[i]);

		//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
		GLint result;
		GLchar errorLog[512];
		glGetShaderiv(vertexshader[i], GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(vertexshader[i], 512, NULL, errorLog);
			std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
			return;
		}
	}
}

void make_fragmentShader()
{
	for (int i = 0; i < 2; i++)
	{
		fragmentsource[i] = filetobuf("fragment.glsl");

		//--- 프래그먼트 세이더 객체 만들기
		fragmentshader[i] = glCreateShader(GL_FRAGMENT_SHADER);

		//--- 세이더 코드를 세이더 객체에 넣기
		glShaderSource(fragmentshader[i], 1, (const GLchar**)&fragmentsource[i], 0);

		//--- 프래그먼트 세이더 컴파일
		glCompileShader(fragmentshader[i]);

		//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
		GLint result;
		GLchar errorLog[512];
		glGetShaderiv(fragmentshader[i], GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(fragmentshader[i], 512, NULL, errorLog);
			std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
			return;
		}
	}
}

void InitBuffer()
{
	glGenVertexArrays(3, vao); //--- VAO 를 지정하고 할당하기
	glGenBuffers(3, vbo); //--- 2개의 VBO를 지정하고 할당하기
	glGenBuffers(2, ebo);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), middle_line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//정육면체
	FILE* fp;
	fp = fopen("cube.obj", "rb");
	ReadObj(fp, cube);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(vec3), cube.vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(FACE), cube.face, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//피라미드
	fp = fopen("pyramid.obj", "rb");
	ReadObj(fp, pyramid);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 5 * sizeof(vec3), pyramid.vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(FACE), pyramid.face, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


}

void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	s_program_plat = glCreateProgram();

	glAttachShader(s_program, vertexshader[0]);
	glAttachShader(s_program, fragmentshader[0]);

	glAttachShader(s_program_plat, vertexshader[1]);
	glAttachShader(s_program_plat, fragmentshader[1]);

	//--- 세이더 삭제하기
	for (int i = 0; i < 2; i++)
	{
		glDeleteShader(vertexshader[i]);
		glDeleteShader(fragmentshader[i]);
	}

	glLinkProgram(s_program);

	// ---세이더가 잘 연결되었는지 체크하기
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(s_program, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(s_program, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return;
	}

	glLinkProgram(s_program_plat);

	// ---세이더가 잘 연결되었는지 체크하기
	glGetProgramiv(s_program_plat, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(s_program_plat, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		glutLeaveMainLoop();
		break;
	}


	glutPostRedisplay();
}

void Timer(int value)
{

	glutTimerFunc(30, Timer, 1);

	glutPostRedisplay();
}

void draw_middle_line()
{
	middle_line[0][0] = 1.f;  middle_line[0][1] = 0;  middle_line[0][2] = 0;
	middle_line[1][0] = -1.f;  middle_line[1][1] = 0;  middle_line[1][2] = 0;
	middle_line[2][0] = 0;  middle_line[2][1] = 1.f;  middle_line[2][2] = 0;
	middle_line[3][0] = 0;  middle_line[3][1] = -1.f;  middle_line[3][2] = 0;
	middle_line[4][0] = 0;  middle_line[4][1] = 0;  middle_line[4][2] = 1.f;
	middle_line[5][0] = 0;  middle_line[5][1] = 0;  middle_line[5][2] = -1.f;
}

void ReadObj(FILE* objFile, READ& Read)
{
	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
	char count[100];
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			Read.vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			Read.faceNum += 1;
		memset(count, '\0', sizeof(count)); // 배열 초기화
	}

	//--- 2. 메모리 할당
	int vertIndex = 0;
	int faceIndex = 0;
	rewind(objFile);
	Read.vertex = (vec3*)malloc(sizeof(vec3) * Read.vertexNum);
	Read.face = (FACE*)malloc(sizeof(FACE) * Read.faceNum);
	//--- 3. 할당된 메모리에 각 버텍스, 페이스 정보 입력
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&Read.vertex[vertIndex].x, &Read.vertex[vertIndex].y,
				&Read.vertex[vertIndex].z);
			vertIndex++;
		}
		else if (count[0] == 'f' && count[1] == '\0') {
			fscanf(objFile, "%d %d %d",
				&Read.face[faceIndex].a, &Read.face[faceIndex].b, &Read.face[faceIndex].c);
			Read.face[faceIndex].a--; Read.face[faceIndex].b--; Read.face[faceIndex].c--;
			faceIndex++;
		}
	}
}