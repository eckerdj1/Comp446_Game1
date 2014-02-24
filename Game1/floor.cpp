#include "floor.h"

Floor::Floor() {

	width = 15.0f;
	length = 15.0f;

}

void Floor::init(ID3D10Device* device) {

	for(int i = 0; i++; i < 10) {
		if(i % 2 == 0) {
			//Solid color
			Box b;
			b.init(
			boxes.push_back(

		} else {
			//Gradient


		}

	}



}

DXColor Floor::getColor1() {
	return boxes[0]->getColor1();
}

DXColor Floor::getColor2(){
	return boxes[0]->getColor2();
}