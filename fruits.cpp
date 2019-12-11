#include "fruits.h"
//#include "GameSpecific.h"
#include "cSoundMgr.h"

static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

fruits::fruits()
{

}

void fruits::add(cTextureMgr* theTxtMgr, std::vector<LPCSTR> txtName, SDL_Point posXY, float fruitRot, int fruitVel, int loopPos)
{
	this->theFruits.push_back(new cFruit(posXY, fruitRot, fruitVel));
	LPCSTR textName = txtName[static_cast<int>(this->theFruits[loopPos]->getEFruitType()) + 1];
	this->theFruits[loopPos]->setTexture(theTxtMgr->getTexture(textName));
	this->theFruits[loopPos]->setSpriteDimensions(theTxtMgr->getTexture(textName)->getTWidth(), theTxtMgr->getTexture(textName)->getTHeight());
	this->theFruits[loopPos]->setFruitVelocity(150);
	this->theFruits[loopPos]->setActive(true);
}

void fruits::update(double fruitDeltaTime)
{
	for (auto anItem : this->theFruits)
	{
		anItem->update(fruitDeltaTime);
	}
}



bool fruits::checkCollisions(SDL_Rect* theBoundingRect, string object) //checks collision based on object
{
	for (auto anItem : this->theFruits)
	{
		if (anItem->collidedWith(&anItem->getBoundingRect(), theBoundingRect))
		{
			if (anItem->isActive() && object == "catcher") //plays sound and disables item
			{
				(theSoundMgr->getSnd("catch")->play(0));
				anItem->setActive(false);
				return true;
			}
			else if (anItem->isActive() && object == "border") //adds to friuts missed, plays sound, and disables item
			{
				fruitsMissed++;
				(theSoundMgr->getSnd("miss")->play(0));
				anItem->setActive(false);
				return true;
			}
			
		}
	}
	return false;
}

int fruits::getfruitsMissed() //gets the fruits missed and returns the int
{
	return fruitsMissed;
}

void fruits::setfruitsMissed(int newfruitsMissed) //sets the fruits missed
{
	fruitsMissed = newfruitsMissed;
}

void fruits::render(SDL_Renderer* theRenderer)
{
	for (auto anItem : this-> theFruits)
	{
		if (anItem->isActive())
		{
			anItem->render(theRenderer, &anItem->getSpriteDimensions(), &anItem->getSpritePos(), anItem->getFruitRotation(), &anItem->getSpriteCentre());
		}
	}
}

int fruits::theNumFruits() //num of fruits
{
	this->numFruits = this->theFruits.size();
	return numFruits;
}

void fruits::clear() //clears fruits for reset
{
	theFruits.clear();
}

int fruits::theNumActiveFruits() //increases fruits active
{
	int active = 0;
	for (auto anItem : this->theFruits)
	{
		if (anItem->isActive() == true)
		{
			active++;
		}
	}
	this->numActiveFruits = active;
	return this->numActiveFruits;
}

SDL_Rect fruits::getFruitsBoundingRect(int loopPos) //fruit collision
{
	return this->theFruits[loopPos]->getBoundingRect();
}

bool fruits::getActive(int loopPos) //gets the fruits active
{
	return this->theFruits[loopPos]->isActive();
}

void fruits::setActive(int loopPos, bool active) //sets the fruits active
{
	this->theFruits[loopPos]->setActive(active);
}

int fruits::getFruitsCaught() //returns fruits caught
{
	return theNumFruits() - theNumActiveFruits() - getfruitsMissed();
}