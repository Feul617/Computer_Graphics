#pragma once
#include <stdio.h>
#include <string.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

typedef struct FACE {
	int a, b, c;
};

void ReadObj(FILE* objFile)
{
	//--- 1. ��ü ���ؽ� ���� �� �ﰢ�� ���� ����
	char count[100];
	int vertexNum = 0;
	int faceNum = 0;
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count)); // �迭 �ʱ�ȭ
	}

	//--- 2. �޸� �Ҵ�
	vec4* vertex;
	FACE* face;
	int vertIndex = 0;
	int faceIndex = 0;
	vertex = (vec4*)malloc(sizeof(vec4) * vertexNum);
	face = (FACE*)malloc(sizeof(FACE) * faceNum);
		//--- 3. �Ҵ�� �޸𸮿� �� ���ؽ�, ���̽� ���� �Է�
		while (!feof(objFile)) {
			fscanf(objFile, "%s", count);
			if (count[0] == 'v' && count[1] == '\0') {
				fscanf(objFile, "%f %f %f",
					&vertex[vertIndex].x, &vertex[vertIndex].y,
					&vertex[vertIndex].z);
				vertIndex++;
			}
			else if (count[0] == 'f' && count[1] == '\0') {
				fscanf(objFile, "%d %d %d",
					&face[faceIndex].a , &face[faceIndex].b, &face[faceIndex].c);
				face[faceIndex].a--; face[faceIndex].b--; face[faceIndex].c--;
				faceIndex++;
			}
		}
}
