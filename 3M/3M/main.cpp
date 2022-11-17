#include "AFX.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void InitBuffer();
void InitShader();

int main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 //--- 윈도우 생성하기
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
		std::cout << "GLEW Initialized\n";;

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//---카메라 설정
	glm::vec4 cameraPos_trans(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 cameraDirection_trans(0.0f, -3.0f, -5.0f, 1.0f);

	glm::vec3 cameraPos = glm::vec3(cameraPos_trans.x, cameraPos_trans.y, cameraPos_trans.z);		 //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(cameraDirection_trans.x, cameraDirection_trans.y, cameraDirection_trans.z); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);		 //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLoc_shape = glGetUniformLocation(s_program, "view"); //--- 뷰잉 변환 설정
	//unsigned int viewLoc_floor = glGetUniformLocation(s_program_floor, "view"); //--- 뷰잉 변환 설정


	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	//큐브


	//glGenVertexArrays(3, vao); //--- VAO 를 지정하고 할당하기
	//glGenBuffers(3, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//glGenBuffers(1, ebo);

	////--- attribute 인덱스 0번을 사용가능하게 함
	//glBindVertexArray(vao[0]);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//glBufferData(GL_ARRAY_BUFFER, cube.faceNum * sizeof(GLfloat), cube.face, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	//glBindVertexArray(vao[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(vec3), cube.vertex, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(FACE), cube.face, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

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

