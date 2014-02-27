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
	void setSpeed(float s) {floorSpeed = s;}
	float getSpeed() {return floorSpeed;}
	int size() {return floor.size();}
	GameObject section(int i) {return floor[i];}
	DXColor getRandomColor();

	void update(float dt);
	void draw(D3DXMATRIX, D3DXMATRIX, ID3D10EffectMatrixVariable*, ID3D10EffectTechnique*);

	DXColor getColor1();
	DXColor getColor2();

private:

	float length;
	int solidLengthSpan, gradientLengthSpan;
	float solidMinLength, gradientMinLength;
	float width;
	float height;
	float floorSpeed;

	void setBoxColor(Box* box);
	
	float screenLength, currentLength;
	float getRandomRGB(float cSet = 2000.0f);

	DXColor nextColor, currentColor, previousColor;
	
	bool channels[3];

	vector<Box*> coloredBoxes;
	vector<GameObject> floor;
	
	ID3D10Device* device;
	// For draw
	D3DXMATRIX mWVP;

};

#endif