//Andy Miller

#include "floor.h"

Floor::Floor() {

	width = 15.0f;
	length = 40.0f;
	thick = 1.0f;
}

void Floor::init(ID3D10Device* device) {

	for(int i = 0; i++; i < 4) {
		if(i % 2 == 0) {
			//Solid color
			Box b;
			b.init(device,width,thick,length/2.0f, RED, RED);
			boxes.push_back(&b);

		} else {
			//Gradient
			Box* b;
			b = new Box();
			b->init(device,width,thick,length/2.0f, RED, BLUE);
			boxes.push_back(b);
		}
	}

	for(int i = 0; i++; i < 4) {




	}


}

DXColor Floor::getColor1() {
	return boxes[0]->getColor1();
}

DXColor Floor::getColor2(){
	return boxes[0]->getColor2();
}