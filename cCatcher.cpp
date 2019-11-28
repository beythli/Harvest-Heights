#include "cCatcher.h"

cCatcher::cCatcher()
{
	this->eCatcherType = catcherType::basket;
	this->catcherPosition = { 0,0 };
	this->setSpritePos(this->catcherPosition);
	this->catcherRotation = 0.0f;
	this->catcherTypeStr = "Apple";
	this->catcherVelocity = 0;
	this->setActive(false);
}

cCatcher::cCatcher(SDL_Point posXY, float theCatcherRot, catcherType theECatcherType, int theVel)
{
	this->eCatcherType = theECatcherType;
	this->catcherPosition = posXY;
	this->setSpritePos(this->catcherPosition);
	this->catcherRotation = theCatcherRot;
	this->setCatcherTypeStr(this->eCatcherType);
	this->catcherVelocity = theVel;
	this->setActive(true);
}

cCatcher::cCatcher(SDL_Point posXY, float theCatcherRot, int theVel)
{
	this->catcherPosition = posXY;
	this->setSpritePos(this->catcherPosition);
	this->catcherRotation = theCatcherRot;
	this->setCatcherTypeStr(this->eCatcherType);
	this->catcherVelocity = theVel;
	this->setActive(true);
}

void cCatcher::update(double catcherDeltaTime)
{
	SDL_Rect currentSpritePos = this->getSpritePos();
	SDL_Point translation = this->getSpriteTranslation();

	currentSpritePos.x += (int)(this->getCatcherVelocity() * catcherDeltaTime);

	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y });
	this->setBoundingRect();
	// The line below is for debugging purposes and can be safely removed.
	std::cout << currentSpritePos.x << "," << currentSpritePos.y << " Rot " << this->getCatcherRotation() << " DT " << catcherDeltaTime << "\n";
}

float cCatcher::getCatcherRotation()
{
	return this->catcherRotation;
}

void cCatcher::setCatcherRotation(float theCatcherRot)
{
	this->catcherRotation = theCatcherRot;
}

SDL_Point cCatcher::getCatcherPos()
{
	return this->catcherPosition;
}

void cCatcher::setCatcherPos(SDL_Point theCatcherPos)
{
	this->catcherPosition = theCatcherPos;
	this->setSpritePos(theCatcherPos);
}

std::string cCatcher::getCatcherTypeStr()
{
	return this->catcherTypeStr;
}

LPCSTR cCatcher::getCatcherTypeStrLPC()
{
	return this->catcherTypeL;
}

void cCatcher::setCatcherTypeStr(std::string theCatcherTypeStr)
{
	this->catcherTypeStr = theCatcherTypeStr;
}

void cCatcher::setCatcherTypeStr(catcherType theCatcherType)
{
	switch (theCatcherType)
	{
	case catcherType::basket:
		this->catcherTypeStr = "basket";
		break;
	case catcherType::trolley:
		this->catcherTypeStr = "trolley";
		break;
	default:
		this->catcherTypeStr = "Error";
	}
	this->catcherTypeL = this->catcherTypeStr.c_str();
}

catcherType cCatcher::getECatcherType()
{
	return this->eCatcherType;
}

void cCatcher::setECatcherType(catcherType theECatcherType)
{
	this->eCatcherType = theECatcherType;
}

int cCatcher::getCatcherVelocity()
{
	return this->catcherVelocity;
}

void cCatcher::setCatcherVelocity(int theCatcherVelocity)
{
	this->catcherVelocity = theCatcherVelocity;
}

