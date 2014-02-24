#pragma once

#include "d3dUtil.h"
#include "constants.h"
#include <string>
#include <sstream>
using std::string;
using std::wstring;
using std::ostringstream;




class Score
{
public:

	Score();
	~Score();

	void init();
	void draw();
	void update(float dt);
	void addPoints(int points);
	void subtractPoints(int points);
	string getString();
	int getInt();

	

private:
	int score;
	string text;
	ostringstream out;

};
	