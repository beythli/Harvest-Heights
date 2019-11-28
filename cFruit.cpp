#include "cFruit.h"

cFruit::cFruit()
{
	this->eFruitType = fruitType::apple;
	this->fruitPosition = { 0,0 };
	this->setSpritePos(this->fruitPosition);
	this->fruitRotation = 0.0f;
	this->fruitTypeStr = "Apple";
	this->fruitValue = 0;
	this->fruitVelocity = 0;
	this->setActive(false);
}

cFruit::cFruit(SDL_Point posXY, float theFruitRot, fruitType theEFruitType, int theVel)
{
	this->eFruitType = theEFruitType;
	this->fruitPosition = posXY;
	this->setSpritePos(this->fruitPosition);
	this->fruitRotation = theFruitRot;
	this->setFruitTypeStr(this->eFruitType);
	this->genFruitValue(this->eFruitType);
	this->fruitVelocity = theVel;
	this->setActive(true);
}

cFruit::cFruit(SDL_Point posXY, float theFruitRot, int theVel)
{
	this->eFruitType = this->genRandomFruitType();
	this->fruitPosition = posXY;
	this->setSpritePos(this->fruitPosition);
	this->fruitRotation = theFruitRot;
	this->setFruitTypeStr(this->eFruitType);
	this->genFruitValue(this->eFruitType);
	this->fruitVelocity = theVel;
	this->setActive(true);
}

void cFruit::update(double fruitDeltaTime)
{
	SDL_Rect currentSpritePos = this->getSpritePos();

	auto rads = PI / 180.0f * this->getFruitRotation();

	this->setFruitRotation((float)(this->getFruitRotation() + (100.0f * fruitDeltaTime)));
	if (this->getFruitRotation() > 360)
	{
		this->setFruitRotation(this->getFruitRotation() - 360.0f);
	}

	currentSpritePos.y += (int)(this->getFruitVelocity() * fruitDeltaTime);

	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y });
	this->setBoundingRect();
	// The line below is for debugging purposes and can be safely removed.
	std::cout << currentSpritePos.x << "," << currentSpritePos.y << " Rot " << this->getFruitRotation() << " DT " << fruitDeltaTime << "\n";

}

float cFruit::getFruitRotation()
{
	return this->fruitRotation;
}

void cFruit::setFruitRotation(float theFruitRot)
{
	this->fruitRotation = theFruitRot;
}

SDL_Point cFruit::getFruitPos()
{
	return this->fruitPosition;
}

void cFruit::setFruitPos(SDL_Point theFruitPos)
{
	this->fruitPosition = theFruitPos;
}

std::string cFruit::getFruitTypeStr()
{
	return this->fruitTypeStr;
}

LPCSTR cFruit::getFruitTypeStrLPC()
{
	return this->fruitTypeL;
}

void cFruit::setFruitTypeStr(std::string theFruitTypeStr)
{
	this->fruitTypeStr = theFruitTypeStr;
}

void cFruit::setFruitTypeStr(fruitType theFruitType)
{
	switch (theFruitType)
	{
	case fruitType::apple:
		this->fruitTypeStr = "apple";
		break;
	case fruitType::carrot:
		this->fruitTypeStr = "carrot";
		break;
	case fruitType::lemon:
		this->fruitTypeStr = "lemon";
		break;
	case fruitType::raspberry:
		this->fruitTypeStr = "raspberry";
		break;
	case fruitType::strawberry:
		this->fruitTypeStr = "strawberry";
		break;
	default:
		this->fruitTypeStr = "Error";
	}
	this->fruitTypeL = this->fruitTypeStr.c_str();
}

fruitType cFruit::getEFruitType()
{
	return this->eFruitType;
}

void cFruit::setEFruitType(fruitType theEFruitType)
{
	this->eFruitType = theEFruitType;
}

int cFruit::getFruitValue()
{
	return this->fruitValue;
}

void cFruit::setFruitValue(int theFruitValue)
{
	this->fruitValue = theFruitValue;
}

int cFruit::getFruitVelocity()
{
	return this->fruitVelocity;
}

void cFruit::setFruitVelocity(int theFruitVelocity)
{
	this->fruitVelocity = theFruitVelocity;
}

fruitType cFruit::genRandomFruitType()
{
	return static_cast<fruitType>(randFruit(gen));
}

void cFruit::genFruitValue(fruitType theFruitType)
{
	switch (theFruitType)
	{
	case fruitType::apple:
		this->fruitValue = 50;
		break;
	case fruitType::carrot:
		this->fruitValue = 75;
		break;
	case fruitType::lemon:
		this->fruitValue = 100;
		break;
	case fruitType::raspberry:
		this->fruitValue = 125;
		break;
	case fruitType::strawberry:
		this->fruitValue = 150;
		break;
	default:
		this->fruitValue = 0;
	}
}
