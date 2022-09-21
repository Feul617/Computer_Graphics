#define Pro 2
#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include<random>

#if Pro == 1

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void TimerFunction(int v);

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0, 1);

int T_Control = 0;

void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
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

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 }

}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	glClearColor(1, 1, 1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}



GLvoid Keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case 'R':
		glClearColor(1, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers(); // 화면에 출력하기
		break;

	case 'G':
		glClearColor(0, 1, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers(); // 화면에 출력하기
		break;

	case 'B':
		glClearColor(0, 0, 1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers(); // 화면에 출력하기
		break;

	case 'A':
		glClearColor(dis(gen), dis(gen), dis(gen), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers(); // 화면에 출력하기
		break;

	case 'W':
		glClearColor(1, 1, 1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers(); // 화면에 출력하기
		break;

	case 'K':
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers(); // 화면에 출력하기
		break;

	case 'T':
		T_Control = 1;
		glutTimerFunc(1000, TimerFunction, 1);
		break;
		
	case 'S':
		T_Control = 0;
		break;

	case 'Q':
		glutLeaveMainLoop();
		break;
	}

	//glutPostRedisplay();
}

void TimerFunction(int v)
{

	if (T_Control == 1) {
		glClearColor(dis(gen), dis(gen), dis(gen), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers(); // 화면에 출력하기
		//glutPostRedisplay();
		glutTimerFunc(100, TimerFunction, 1);
	}

	else if (T_Control == 0) {
		glClearColor(dis(gen), dis(gen), dis(gen), 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers(); // 화면에 출력하기
	}

}

#elif Pro == 2

#define WIDHT 800
#define HEIGHT 600

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void Mouse(int button, int state, int x, int y);

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(0, 1);

GLfloat r = dis(gen), g = dis(gen), b = dis(gen);
GLfloat back_r = dis(gen), back_g = dis(gen), back_b = dis(gen);
GLfloat f_x = -0.5f, s_x = 0.5f;
GLfloat f_y = -0.5f, s_y = 0.5f;

void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(WIDHT, HEIGHT); // 윈도우의 크기 지정
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

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop(); // 이벤트 처리 시작 }

}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	glClearColor(back_r, back_g, back_b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(r, g, b);
	glRectf(f_x, f_y, s_x, s_y);

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDHT / 2)) / (WIDHT / 2);
	GLfloat real_y = (GLfloat)(y - (HEIGHT / 2)) / (HEIGHT / 2);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (real_x > f_x && real_x < s_x && real_y > f_y && real_y < s_y) {
			r = dis(gen);
			g = dis(gen);
			b = dis(gen);
		}
		else {
			back_r = dis(gen);
			back_g = dis(gen);
			back_b = dis(gen);

			glColor3f(r, g, b);
			glRectf(f_x, f_y, s_x, s_y);
		}
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (real_x > f_x && real_x < s_x && real_y > f_y && real_y < s_y) {
			f_x -= 0.05;
			f_y -= 0.05;
			s_x += 0.05;
			s_y += 0.05;
		}
		else {
			f_x += 0.05;
			f_y += 0.05;
			s_x -= 0.05;
			s_y -= 0.05;
		}
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	
}

#endif