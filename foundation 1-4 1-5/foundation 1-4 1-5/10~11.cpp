#define Pro 10
#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "file.h"
#include <random>

#define WIDTH 800
#define HEIGHT 600

#if Pro == 10

//����
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader, vao[6], vbo[6];
GLuint s_program, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLfloat point_shape[2] = {};
GLfloat line_shape[3][2] = {};
GLfloat middle_line[4][2] = {};
GLfloat tri_shape[2][3][2] = {};
GLfloat rect_shape[2][4][2] = {};
GLfloat penta_shape[2][5][2] = {};

GLfloat colors[4][3] = {};

bool change = false;
int animation = 0;

//�Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();

//�׸��� �Լ�
void draw_middle_line();
void draw_line();

//�����̱� �Լ�
void change_line();

void Keyboard(unsigned char key, int x, int y);
void Timer(int value);


void main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� //--- ������ �����ϱ�
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

	draw_middle_line();
	draw_line();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
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
	glBindVertexArray(vao[5]);
	glEnableVertexAttribArray(0);

	//--- �ﰢ�� �׸���
	for (int i = 0; i < 4; i++) {
		glDrawArrays(GL_LINES, i * 2, 2);
	}

	glBindVertexArray(vao[1]);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

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
	glGenVertexArrays(6, vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(6, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao[0]); //--- VAO�� ���ε��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat), point_shape, GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), line_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tri_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), rect_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), penta_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), middle_line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	////--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(0);

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

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		change = !change;

		glutTimerFunc(30, Timer, 1);
		break;
	}
	glutPostRedisplay();
}

void Timer(int value)
{
	change_line();

	if (change) {
		animation += 1;
	}

	else if (!change) {
		animation -= 1;
	}


	glutPostRedisplay(); // ȭ�� �� ���
	if (animation == 11) {
		animation = 10;
		return;
	}

	else if (animation == -1) {
		animation = 0;
		return;
	}

	glutTimerFunc(30, Timer, 1);
}

void draw_middle_line()
{
	//���μ�
	middle_line[0][0] = -1.f;   middle_line[1][0] = 1.f;  
	middle_line[0][1] = middle_line[1][1] = 0; 

	//���μ�
	middle_line[2][0] = middle_line[3][0] = 0;
	middle_line[2][1] = 1.f;  middle_line[3][1] = -1.f;
}

void draw_point()
{

}

void draw_line()
{
	line_shape[0][0] = -0.8f;  line_shape[0][1] = 0.2f;  //���� ��
	line_shape[1][0] = -0.2f;  line_shape[1][1] = 0.8f;  //������ ��
	line_shape[2][0] = -0.2f;  line_shape[2][1] = 0.8f;  //�����ִ� ��
}

void draw_triangle()
{

}

void draw_rectangle()
{

}

void draw_pentagon()
{

}

void change_line()
{
	if(change && animation < 10)
		line_shape[2][1] -= 0.06f;

	else if(!change && animation > 0)
		line_shape[2][1] += 0.06f;
}



#elif Pro == 11



#endif