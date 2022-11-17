#pragma once
#include <iostream>
#include <random>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include <gl/glew.h>
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>
//#include <glm/glm/glm.hpp>
//#include <glm/glm/ext.hpp>
//#include <glglm/glm/gtc/matrix_transform.hpp>


#define _CRT_SECURE_NO_WARNINGS

using namespace glm;
using namespace std;

#define WIDTH 1600
#define HEIGHT 900

typedef struct Vector2 {
	GLfloat x;
	GLfloat y;
}Vector2;

typedef struct Vector3 {
	GLfloat x;
	GLfloat y;
	GLfloat z;
}Vector3;

typedef struct FACE {
	int a, b, c;
};

typedef struct READ {
	int vertexNum;
	int faceNum;
	vec3* vertex;
	FACE* face;
}READ;

//변수
extern GLuint vertexshader, fragmentshader;
extern GLuint s_program, s_program_floor, shaderID;
extern GLfloat rColor, bColor, gColor;
extern GLint result;
extern GLchar errorLog[512];

extern FILE* fp;

extern vec3 plat[4];

//함수
void make_vertexShader();
void make_fragmentShader();

void ReadObj(FILE* objFile, READ& Read);

char* filetobuf(const char* file);