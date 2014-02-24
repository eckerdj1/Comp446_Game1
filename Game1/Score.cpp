#include "Score.h"

Score::Score()
{
	score = 0;
}
Score::~Score()
{

}

void Score::init()
{

}

void Score::draw()
{

}

void Score::addPoints(int points)
{
	score += points;
}

void Score::subtractPoints(int points)
{
	score -= points;
}

string Score::getString()
{
	out.str("");
	out << "Lava Blocks Destroyed: " << score;
	return out.str();
}

int Score::getInt()
{
	return score;
}