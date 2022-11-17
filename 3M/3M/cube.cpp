#include "cube.h"

READ* Cube::cube = nullptr;

Cube::Cube()
{
	if (cube == nullptr) {
		cube = new READ;
		fp = fopen("Cube.obj", "rb");
		ReadObj(fp, *cube);
	}
}

Cube::~Cube()
{
}

void Cube::Init()
{
	//glUseProgram(s_program);

	//glGenVertexArrays(1, &VAO); //--- VAO 를 지정하고 할당하기
	//glGenBuffers(1, &VBO); //--- 2개의 VBO를 지정하고 할당하기
	//glGenBuffers(1, &EBO);

	////--- attribute 인덱스 0번을 사용가능하게 함
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//glBufferData(GL_ARRAY_BUFFER, cube->vertexNum * sizeof(GLfloat), cube->vertex, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube->faceNum * sizeof(FACE), cube->face, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	//glEnableVertexAttribArray(0);
}

void Cube::Update()
{
}

void Cube::Draw()
{
}
