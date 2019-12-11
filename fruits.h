#pragma once
#include "cFruit.h"
#include <vector>
//#include "GameSpecific.h"
//#include "cGame.h"

class fruits
{
private:
	std::vector <cFruit*> theFruits;
	int numFruits = 0;
	int numActiveFruits = 0;
	int fruitsCaught = 0;

public:
	fruits();
	void add(cTextureMgr* theTxtMgr, std::vector<LPCSTR> txtName, SDL_Point posXY, float fruitRot, int fruitVel, int loopPos);
	void update(double fruitDeltaTime);
	bool checkCollisions(SDL_Rect* theBoundingRect, string object);
	void render(SDL_Renderer* theRenderer);
	void clear();

	//used for score and fruits missed 
	int getFruitsCaught();
	int theNumFruits();
	int theNumActiveFruits();
	int getfruitsMissed();
	void setfruitsMissed(int newfruitsMissed);
	int fruitsMissed = 0;

	SDL_Rect getFruitsBoundingRect(int loopPos);
	bool getActive(int loopPos);
	void setActive(int loopPos, bool active);
};

