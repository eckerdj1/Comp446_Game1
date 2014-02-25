//Andy Miller

#ifndef FLOOR_H
#define FLOOR_H

#include "d3dUtil.h"
#include "Box.h"
#include "GameObject.h"
#include <vector>
using std::vector;

class Floor
{
public:
	Floor();

	void init(ID3D10Device* device);

	DXColor getColor1();
	DXColor getColor2();

private:

	float length;
	float width;
	float thick;

	DXColor color1, color2;

	vector<Box*> boxes;
	vector<GameObject> boxObjects;
	

};

#endif