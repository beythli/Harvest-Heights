#pragma once
#include "cSprite.h"
class cCatcher : public cSprite
{
private:
	float catcherRotation = 0.0f;
	SDL_Point catcherPosition = { 0,0 };
	std::string catcherTypeStr = "";
	LPCSTR catcherTypeL = catcherTypeStr.c_str();
	catcherType eCatcherType = catcherType::basket;
	int catcherVelocity = 0;

public:
	cCatcher();
	cCatcher(SDL_Point posXY, float theCatcherRot, catcherType theECatcherType, int theVel);
	cCatcher(SDL_Point posXY, float theCatcherRot, int theVel);
	void update(double catcherDeltaTime);
	float getCatcherRotation();
	void setCatcherRotation(float theCatcherRot);
	SDL_Point getCatcherPos();
	void setCatcherPos(SDL_Point theCatcherPos);
	std::string getCatcherTypeStr();
	LPCSTR getCatcherTypeStrLPC();
	void setCatcherTypeStr(std::string theCatcherTypeStr);
	void setCatcherTypeStr(catcherType theCatcherType);
	catcherType getECatcherType();
	void setECatcherType(catcherType theECatcherType);
	int getCatcherVelocity();
	void setCatcherVelocity(int theCatcherVelocity);
};

