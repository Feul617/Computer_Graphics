#define Pro 7
#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "file.h"

#define WIDTH 800
#define HEIGHT 600

#if Pro == 6

//����
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader, vao, vbo;
GLuint s_program, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLfloat size_x = 0.1f;
GLfloat size_y = 0.15f;

GLfloat center_verx, center_very; //�ﰢ�� �߽� ��ǥ
GLfloat interval_x = 0.2f, interval_y = 0.3f; //�� ������ �Ÿ�
int tri_select = 0;

GLfloat triShape[4][3][2] = { //--- �ﰢ�� ��ġ ��
0};

GLfloat colors[4][3] = {};


//�Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();
void draw_triangle();

void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);


void main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� { //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WIDTH, HEIGHT); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	else
		std::cout << "GLEW Initialized\n";


	draw_triangle();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // �̺�Ʈ ó�� ���� }
}

GLvoid drawScene()//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // �������� ��blue�� �� ����
{
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");
	//--- ����� ���� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);

	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);

	//--- �ﰢ�� �׸���
	for (int i = 0; i < 4; i++) {
		glUniform3f(vColorLocation, colors[i][0], colors[i][1], colors[i][2]);
		glDrawArrays(GL_TRIANGLES, i * 3, 3);
	}

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�

}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� {
{
	glViewport(0, 0, w, h);
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");

	//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);

	//--- �������� ����� ���� ���� ���: ���� üũ

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if(!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");

	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);

	//--- �������� ����� ���� ���� ���: ������ ���� üũ

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�

	glGenBuffers(1, &vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	////--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	////--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	////--- colors �迭�� ������: 9 *float 
	//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	////--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	////--- attribute �ε��� 1���� ��� �����ϰ� ��.
	//glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShader(); //--- ���ؽ� ���̴� �����

	make_fragmentShader(); //--- �����׸�Ʈ ���̴� �����

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");
	// 
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDTH / 2)) / (WIDTH / 2);
	GLfloat real_y = (GLfloat)-(y - (HEIGHT / 2)) / (HEIGHT / 2);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		interval_x += size_x;
		interval_y += size_y;
		triShape[tri_select][0][0] = real_x + interval_x ; triShape[tri_select][0][1] = real_y - interval_y;   //������ ��
		triShape[tri_select][1][0] = real_x - interval_x; triShape[tri_select][1][1] = real_y - interval_y;   //���� ��
		triShape[tri_select][2][0] = real_x; triShape[tri_select][2][1] = real_y + interval_y;	      //���� ��
		if (interval_x > (GLfloat)0.4f || interval_x < 0.2f) {
			size_x *= -1;
			size_y *= -1;
		}

		tri_select++;
		tri_select %= 4;
		std::cout << interval_x << std::endl;
		std::cout << tri_select << std::endl;
		std::cout << real_x << " " << real_y << std::endl;
	}
	InitBuffer();
	glutPostRedisplay();
}

void draw_triangle()
{
	center_verx = -0.5f;
	center_very = 0.5f;
	for (int i = 0; i < 4; i++) {
		if (i == 2)
			center_very *= -1;

		triShape[i][0][0] = center_verx + 0.2f; triShape[i][0][1] = center_very - 0.3f;   //������ ��
		triShape[i][1][0] = center_verx - 0.2f; triShape[i][1][1] = center_very - 0.3f;   //���� ��
		triShape[i][2][0] = center_verx; triShape[i][2][1] = center_very + 0.3f;	      //���� ��

		std::cout << triShape[i][0][0] << " " << triShape[i][0][1] << std::endl;
		std::cout << triShape[i][1][0] << " " << triShape[i][1][1] << std::endl;
		std::cout << triShape[i][2][0] << " " << triShape[i][2][1] << std::endl;
		std::cout << "\n" << std::endl;

		center_verx *= -1;
		if(i < 3 )
			colors[i][i] = 1.f;
	}
	colors[3][0] = 0.5f; colors[3][1] = 0.5f;
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		std::cout << "a" << std::endl;
		break;

	case 'b':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::cout << "b" << std::endl;
		break;
	}
	glutPostRedisplay();
}

#elif Pro == 7

//����
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader, vao, vbo;
GLuint s_program, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLfloat center_verx[4] = { -0.5f, 0.5f ,-0.5f ,0.5f }, center_very[4] = { 0.5f, 0.5f, -0.5f, -0.5f }; //�ﰢ�� �߽� ��ǥ
GLfloat interval_x = 0.2f, interval_y = 0.3f; //�� ������ �Ÿ�

GLfloat move_x[4] = { 0.1f, -0.1f, 0.1f, -0.1f };
GLfloat move_y[4] = { 0.1f, 0.1f, -0.1f, -0.1f };

GLfloat triShape[4][3][2] = {}; //--- �ﰢ�� ��ġ ��
GLfloat colors[4][3] = {};

bool start = false;

//�Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();
void draw_triangle();
void move_triangle();

void Keyboard(unsigned char key, int x, int y);


void main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� { //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WIDTH, HEIGHT); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	else
		std::cout << "GLEW Initialized\n";


	draw_triangle();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	move_triangle();
	glutMainLoop(); // �̺�Ʈ ó�� ���� }
}

GLvoid drawScene()//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // �������� ��blue�� �� ����
{
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");
	//--- ����� ���� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);

	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);

	//--- �ﰢ�� �׸���
	for (int i = 0; i < 4; i++) {
		glUniform3f(vColorLocation, colors[i][0], colors[i][1], colors[i][2]);
		glDrawArrays(GL_TRIANGLES, i * 3, 3);
	}

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�

}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� {
{
	glViewport(0, 0, w, h);
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");

	//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);

	//--- �������� ����� ���� ���� ���: ���� üũ

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");

	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);

	//--- �������� ����� ���� ���� ���: ������ ���� üũ

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�

	glGenBuffers(1, &vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	////--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	////--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	////--- colors �迭�� ������: 9 *float 
	//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	////--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	////--- attribute �ε��� 1���� ��� �����ϰ� ��.
	//glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShader(); //--- ���ؽ� ���̴� �����

	make_fragmentShader(); //--- �����׸�Ʈ ���̴� �����

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");
	// 
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

void draw_triangle()
{
	for (int i = 0; i < 4; i++) {

		triShape[i][0][0] = center_verx[i] + 0.2f; triShape[i][0][1] = center_very[i] - 0.3f;   //������ ��
		triShape[i][1][0] = center_verx[i] - 0.2f; triShape[i][1][1] = center_very[i] - 0.3f;   //���� ��
		triShape[i][2][0] = center_verx[i]; triShape[i][2][1] = center_very[i] + 0.3f;	      //���� ��

		std::cout << triShape[i][0][0] << " " << triShape[i][0][1] << std::endl;
		std::cout << triShape[i][1][0] << " " << triShape[i][1][1] << std::endl;
		std::cout << triShape[i][2][0] << " " << triShape[i][2][1] << std::endl;
		std::cout << "\n" << std::endl;

		if (i < 3)
			colors[i][i] = 1.f;
	}
	colors[3][0] = 0.5f; colors[3][1] = 0.5f;
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		start = true;
		glutTimerFunc(Timer);
		break;
	}
	glutPostRedisplay();
}


void move_triangle()
{
	if (start == true) {
		for (int i = 0; i < 4; i++) {
			center_verx[i] += move_x[i]; center_very[i] += move_y[i];

			if (triShape[i][0][0] > 1.f || triShape[i][0][0] < -1.f) {
				move_x[i] *= -1;
			}
		}
	}

	glutPostRedisplay();
}

void Timer(int value)
{

}

#endif