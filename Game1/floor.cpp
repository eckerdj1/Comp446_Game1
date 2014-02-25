//Andy Miller

#include "floor.h"

Floor::Floor() {
	screenLength = 200.0f;
	currentLength = 0.0f;

	width = 30.0f;
	length = 40.0f;
	height = 1.95f;
	solidLengthSpan = 25;
	solidMinLength = 25.0f;
	gradientLengthSpan = 15;
	gradientMinLength = 15.0f;
	D3DXMatrixIdentity(&mWVP); 
	floorSpeed = 17.0f;
}

void Floor::init(ID3D10Device* device) 
{
	this->device = device;
	previousColor = BLACK;
	float r, g, b;
	r = getRandomRGB();
	g = getRandomRGB();
	b = getRandomRGB();
	currentColor = DXColor(r, g, b, 1.0f);
	while(currentLength < screenLength)
	{
		
		Box* box = new Box();
		if (coloredBoxes.size() % 2 == 0) // solid color
		{
			length = rand() % solidLengthSpan + solidMinLength;
			box->init(device, width / 2.0f, height / 2.0f, length / 2.0f, currentColor, currentColor);
			previousColor = currentColor;
		}
		else // gradient color change
		{
			int pick = rand() % 3;
			r = currentColor.r;
			g = currentColor.g;
			b = currentColor.b;
			switch(pick)
			{
			case 0:
				r = getRandomRGB();
				break;
			case 1:
				g = getRandomRGB();
				break;
			case 2:
				b = getRandomRGB();
				break;
			}
			nextColor = DXColor(r, g, b, 1.0f);
			length = rand() % gradientLengthSpan + gradientMinLength;
			box->init(device, width / 2.0f, height / 2.0f, length / 2.0f, currentColor, nextColor);
			previousColor = currentColor;
			currentColor = nextColor;
		}
		currentLength += length;
		coloredBoxes.push_back(box);

		GameObject floorSection;
		floorSection.init(coloredBoxes.back(), 1.0f, Vector3(0, -2, 10 + currentLength - (length / 2.0f)), Vector3(0, 0, -1), floorSpeed, length);
		floor.push_back(floorSection);
	}
	//for(int i = 0; i++; i < 4) {
	//	if(i % 2 == 0) {
	//		//Solid color
	//		Box b;
	//		b.init(device,width,height,length/2.0f, RED, RED);
	//		boxes.push_back(&b);

	//	} else {
	//		//Gradient
	//		Box* b;
	//		b = new Box();
	//		b->init(device,width,height,length/2.0f, RED, BLUE);
	//		boxes.push_back(b);
	//	}
	//}

	//for(int i = 0; i++; i < 4) {




	//}


}

DXColor Floor::getColor1() {
	return coloredBoxes[0]->getColor1();
}

DXColor Floor::getColor2(){
	return coloredBoxes[0]->getColor2();
}

float Floor::getRandomRGB(float cSet)
{
	return (rand() % (int)cSet) / cSet;
}

void Floor::update(float dt)
{
	for (int i=0; i<floor.size(); ++i)
	{
		float zPos = floor[i].getPosition().z + floor[i].getScale() / 2.0f;
		if (zPos < -30.0f)
		{
			floor[i].deleteBox();
			delete coloredBoxes[i];
			Box* b = getNewBox();
			coloredBoxes[i] = b;
			floor[i].setBox(b);
			currentLength -= floor[i].getScale();
			zPos += currentLength + (length / 2.0f);
			floor[i].setPosition(Vector3(0, -1, zPos));
			floor[i].setScale(length);
			currentLength += length;
		}
		floor[i].update(dt);
	}
}

void Floor::draw(D3DXMATRIX view, D3DXMATRIX proj, ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* tech)
{
	for (int i=0; i<floor.size(); ++i)
	{
		mWVP = floor[i].getWorldMatrix() * view * proj;
		mfxWVPVar->SetMatrix((float*)&mWVP);
		floor[i].setMTech(tech);
		floor[i].draw();
	}
}

Box* Floor::getNewBox()
{
	Box* box = new Box();
	float r, g, b;
	if (previousColor != currentColor) //next one is solid
	{
		length = rand() % solidLengthSpan + solidMinLength;
		box->init(device, width / 2.0f, height / 2.0f, length / 2.0f, currentColor, currentColor);
		previousColor = currentColor;
	}
	else //next one is a gradient
	{
		int pick = rand() % 3;
		r = currentColor.r;
		g = currentColor.g;
		b = currentColor.b;
		switch(pick)
		{
		case 0:
			r = getRandomRGB();
			break;
		case 1:
			g = getRandomRGB();
			break;
		case 2:
			b = getRandomRGB();
			break;
		}
		nextColor = DXColor(r, g, b, 1.0f);
		length = rand() % gradientLengthSpan + gradientMinLength;
		box->init(device, width / 2.0f, height / 2.0f, length / 2.0f, currentColor, nextColor);
		previousColor = currentColor;
		currentColor = nextColor;
	}
	return box;
}