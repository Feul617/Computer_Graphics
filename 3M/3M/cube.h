#pragma once
#include "AFX.h"

class Cube {
public:
	static READ* cube;

public:
	Cube();
	~Cube();

public:
	void Init();
	void Update();
	void Draw();

public:
	vec3 position;
	vec3 scale;

private:
	//GLuint VAO, VBO, EBO;
};