//Andy Miller

#include "floor.h"

Floor::Floor() {
	screenLength = 200.0f;
	currentLength = 0.0f;

	width = 30.0f;
	length = 40.0f;
	height = 1.95f;
	fudgeFactor = 1.001f;

	solidLengthSpan = 25;
	solidMinLength = 25.0f;
	gradientLengthSpan = 15;
	gradientMinLength = 15.0f;
	D3DXMatrixIdentity(&mWVP); 
	floorSpeed = 17.0f;
	channels[0] = false;
	channels[1] = false;
	channels[2] = false;
}

void Floor::init(ID3D10Device* device) 
{
	this->device = device;
	previousColor = BLACK;
	



	currentColor = getRandomColor();
	while(currentLength < screenLength)
	{
		
		Box* box = new Box();
		if (coloredBoxes.size() % 2 == 0) // solid color
		{
			length = rand() % solidLengthSpan + solidMinLength;
			box->init(device, width / 2.0f, height / 2.0f, length / 1.999f, currentColor, currentColor);
			previousColor = currentColor;
		}
		else // gradient color change
		{
			nextColor = getRandomColor();
			length = rand() % gradientLengthSpan + gradientMinLength;
			box->init(device, width / 2.0f, height / 2.0f, length / 2.0f, currentColor, nextColor);
			previousColor = currentColor;
			currentColor = nextColor;
		}
		currentLength += length;
		coloredBoxes.push_back(box);

		GameObject floorSection;
		floorSection.init(coloredBoxes.back(), 1.0f, Vector3(0, -2, -30 + currentLength - (length / 2.0f)), Vector3(0, 0, -1), floorSpeed, Vector3(width, height,length));
		floor.push_back(floorSection);
	}
}

DXColor Floor::getColor1() {
	return coloredBoxes[0]->getColor1();
}

DXColor Floor::getColor2(){
	return coloredBoxes[0]->getColor2();
}

DXColor Floor::getRandomColor()
{
	DXColor color;
	float r, g, b;
	int pick = rand() % 3;
	int count = 0;
	float val = 0.0f;
	for (int i=0; i<3; ++i)
		channels[i] = false;
	while(count < 3)
	{
		if (count <= 1)
			val = (float)count;
		else
			val = (float)(rand() % 255) / 255.0f;

		while(channels[pick])
		{
			pick = rand() % 3;
		}
		channels[pick] = true;
		switch(pick)
		{
		case 0:	// red
			r = val;
			break;
		case 1:
			g = val;
			break;
		case 2:
			b = val;
			break;
		}
		count++;
	}
	color = DXColor(r, g, b, 1.0f);
	return color;
}

float Floor::getRandomRGB(float cSet)
{
	return (rand() % (int)cSet) / cSet;
}

void Floor::update(float dt)
{
	for (int i=0; i<floor.size(); ++i)
	{
		float zPos = floor[i].getPosition().z + floor[i].zRadius();
		if (zPos < -30.0f)
		{
			setBoxColor(coloredBoxes[i]);
			currentLength -= floor[i].getSize().z;
			zPos += currentLength + (length / 2.0f);
			floor[i].setPosition(Vector3(0, -2, zPos));
			floor[i].setSize(Vector3(width, height, length));
			currentLength += length;
		}
		floor[i].setSpeed(floorSpeed);
		floor[i].update(dt);
	}
}

void Floor::draw(D3DXMATRIX view, D3DXMATRIX proj, ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectMatrixVariable* mfxWorldVar, ID3D10EffectTechnique* tech)
{
	for (int i=0; i<floor.size(); ++i)
	{
		mWVP = floor[i].getWorldMatrix() * view * proj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mfxWorldVar->SetMatrix((float*)&floor[i].getWorldMatrix());
		floor[i].setMTech(tech);
		floor[i].draw();
	}
}

void Floor::setBoxColor(Box* box)
{
	float r, g, b;
	box->releaseVBuffer();
	if (previousColor != currentColor) //next one is solid
	{
		length = rand() % solidLengthSpan + solidMinLength;
		box->init(device, width / 2.0f, height / 2.0f, length / 2.0f, currentColor, currentColor);
		previousColor = currentColor;
	}
	else //next one is a gradient
	{
		nextColor = getRandomColor();
		length = rand() % gradientLengthSpan + gradientMinLength;
		box->init(device, width / 2.0f, height / 2.0f, length / 2.0f, currentColor, nextColor);
		previousColor = currentColor;
		currentColor = nextColor;
	}
}