#pragma once
#include "cube.h"

class Map {
public:
	Map();
	~Map();

public:
	void Init();
	void Update();

public:
	int x, y, z;
	Cube** block;
};