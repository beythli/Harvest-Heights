#pragma once
#include "GameConstants.h"
#include <random>
#include "cSprite.h"
class cFruit : public cSprite
{
private:
	float fruitRotation = 0.0f;
	SDL_Point fruitPosition = {0,0};
	std::string fruitTypeStr = "";
	LPCSTR fruitTypeL = fruitTypeStr.c_str();
	fruitType eFruitType = fruitType::apple;
	int fruitValue = 0;
	int fruitVelocity = 0;

	/* Let the computer pick a random number */
	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() }; // deterministic engine. For most common uses, std::mersenne_twister_engine, fast and high-quality.
	uniform_int_distribution<> randFruit{0, 4 }; // Range apple to strawberry

public:
	cFruit();
	cFruit(SDL_Point posXY, float theFruitRot, fruitType theEFruitType, int theVel);
	cFruit(SDL_Point posXY, float theFruitRot, int theVel);
	void update(double fruitDeltaTime);
	float getFruitRotation();
	void setFruitRotation(float theFruitRot);
	SDL_Point getFruitPos();
	void setFruitPos(SDL_Point theFruitPos);
	std::string getFruitTypeStr();
	LPCSTR getFruitTypeStrLPC();
	void setFruitTypeStr(std::string theFruitTypeStr);
	void setFruitTypeStr(fruitType theFruitType);
	fruitType getEFruitType();
	void setEFruitType(fruitType theEFruitType);
	int getFruitValue();
	void setFruitValue(int theFruitValue);
	int getFruitVelocity();
	void setFruitVelocity(int theFruitVelocity);
	fruitType genRandomFruitType();
	void genFruitValue(fruitType theFruitType);
};

