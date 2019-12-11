/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"
#include "SDL_mixer.h"

#include <sstream>
cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();



/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();

	
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();


	// Store the textures
	textureName = { "theBackground", "apple", "carrot" , "lemon" , "raspberry", "strawberry" , "basket", "openingScreen", "endScreen", "house", "border", "howToPlay", "barn", "cow", "chicken", "chicken2","tree", "tree2", "tractor"};
	texturesToUse = { "Images/Bkg/Background.jpg", "Images/Sprites/Apple.png", "Images/Sprites/Carrot.png" , "Images/Sprites/Lemon.png" , "Images/Sprites/Raspberry.png", "Images/Sprites/Strawberry.png" , "Images/Sprites/Basket64x64.png", 
					"../Images/Bkg/openingScreen.jpg", "../Images/Bkg/endScreen.jpg", "../Images/Sprites/House.png", "../Images/Sprites/Border.png", "../Images/Bkg/HowToPlay2.png", "../Images/Sprites/Barn.png", "../Images/Sprites/Cow.png", 
					"../Images/Sprites/Chicken.png", "../Images/Sprites/Chicken2.png", "../Images/Sprites/tree.png", "../Images/Sprites/tree.png", "../Images/Sprites/Tractor.png" };
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	aColour = { 228, 213, 238, 255 };
	

	// Create textures for Game Dialogue (text)
	fontList = { "fruit", "score"};
	fontsToUse = { "Fonts/FarmToMarketFancy-n8P1.ttf", "Fonts/digital-7.ttf"};
	for (unsigned int fonts = 0; fonts < fontList.size(); fonts++)
	{
		if (fonts == 0)
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 48);
		}
		else
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
		}
	}

	// Create text Textures
	gameTextNames = {"MenuTitle"};
	gameTextList = {"Harvest Heights"};
	for (unsigned int text = 0; text < gameTextNames.size(); text++)
	{
		if (text == 0 || text == gameTextNames.size()-1)
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("fruit")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		}
		else 
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("score")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		}
	}

	//second list
	gameTextNames = { "score", "health", "gameOver" };
	gameTextList2 = { "Caught: ", "Missed: ", "Game Over! You Caught: " };
	for (unsigned int text = 0; text < gameTextNames.size(); text++)
	{
		if (text == 0 || text == gameTextNames.size() - 1)
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("score")->createTextTexture(theRenderer, gameTextList2[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		} 
		else
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("fruit")->createTextTexture(theRenderer, gameTextList2[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		}
	}
	theTextureMgr->deleteTexture("score");
	theTextureMgr->deleteTexture("health");
	

	// Load game sounds
	soundList = { "theme", "catch", "menu", "miss"};
	soundTypes = { soundType::music, soundType::sfx, soundType::music, soundType::sfx };
	soundsToUse = { "../Audio/Theme/2AM_Boogie.wav", "../Audio/SFX/LevelUp_Catch.wav", "../Audio/Theme/Good_Evening.wav",  "../Audio/SFX/Error Sound.wav" };
	for (unsigned int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}


	//buttons
	btnNameList = { "quit_btn", "howtoplay_btn", "menu_btn", "play_btn", "save_btn" };
	btnTexturesToUse = { "../Images/Buttons/button_quit2.png", "../Images/Buttons/button_howtoplay.png", "../Images/Buttons/button_menu.png", "../Images/Buttons/button_play2.png", "Images/Buttons/button_save.png" };
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 400, 300 }, { 740, 500 }, { 400, 300 } };
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton* newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theBtnType = btnTypes::exit;

	//sprites to add
	house.setSpritePos({ 89, 285 });
	house.setTexture(theTextureMgr->getTexture("house"));
	house.setSpriteDimensions(theTextureMgr->getTexture("house")->getTWidth(), theTextureMgr->getTexture("house")->getTHeight());
	
	barn.setSpritePos({ 800, 280 });
	barn.setTexture(theTextureMgr->getTexture("barn"));
	barn.setSpriteDimensions(theTextureMgr->getTexture("barn")->getTWidth(), theTextureMgr->getTexture("barn")->getTHeight());
	
	cow.setSpritePos({ 715, 400 });
	cow.setTexture(theTextureMgr->getTexture("cow"));
	cow.setSpriteDimensions(theTextureMgr->getTexture("cow")->getTWidth(), theTextureMgr->getTexture("cow")->getTHeight());

	chicken.setSpritePos({ 900, 470 });
	chicken.setTexture(theTextureMgr->getTexture("chicken"));
	chicken.setSpriteDimensions(theTextureMgr->getTexture("chicken")->getTWidth(), theTextureMgr->getTexture("chicken")->getTHeight());
	
	chicken2.setSpritePos({ 800, 500 });
	chicken2.setTexture(theTextureMgr->getTexture("chicken2"));
	chicken2.setSpriteDimensions(theTextureMgr->getTexture("chicken2")->getTWidth(), theTextureMgr->getTexture("chicken2")->getTHeight());

	tree.setSpritePos({ 0, 350 });
	tree.setTexture(theTextureMgr->getTexture("tree"));
	tree.setSpriteDimensions(theTextureMgr->getTexture("tree")->getTWidth(), theTextureMgr->getTexture("tree")->getTHeight());

	tree2.setSpritePos({ 250, 345 });
	tree2.setTexture(theTextureMgr->getTexture("tree2"));
	tree2.setSpriteDimensions(theTextureMgr->getTexture("tree2")->getTWidth(), theTextureMgr->getTexture("tree2")->getTHeight());

	tractor.setSpritePos({ 375, 425 });
	tractor.setTexture(theTextureMgr->getTexture("tractor"));
	tractor.setSpriteDimensions(theTextureMgr->getTexture("tractor")->getTWidth(), theTextureMgr->getTexture("tractor")->getTHeight());

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	theCatcher.setCatcherPos({ 500, 600 });
	theCatcher.setTexture(theTextureMgr->getTexture("basket"));
	theCatcher.setSpriteDimensions(theTextureMgr->getTexture("basket")->getTWidth(), theTextureMgr->getTexture("basket")->getTHeight());

	border.setSpritePos({ 0, 729 });
	border.setTexture(theTextureMgr->getTexture("border"));
	border.setSpriteDimensions(theTextureMgr->getTexture("border")->getTWidth(), theTextureMgr->getTexture("border")->getTHeight());

	//converts score to a string
	strScore = gameTextList2[0];
	strScore += to_string(fruitsCaught).c_str();

	//converts fruits missed to a string
	strHealth = gameTextList2[1];
	strHealth += to_string(theFruits.getfruitsMissed()).c_str();

	std::cout << "Initialisation complete!\n\n";
}

void cGame::run(SDL_Renderer* theRenderer)
{

	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theRenderer);
	}
}

void cGame::render(SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	SDL_Rect theTextureRectangle = {0,0,0,0};
	SDL_Rect theButtonDims = { 0,0,0,0 };
	SDL_Rect theButtonPos = { 0,0,0,0 };

	switch (theGameState)
	{
	case gameState::menu: //menu background, title, and buttons
	{

		spriteBkgd.setTexture(theTextureMgr->getTexture("openingScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("openingScreen")->getTWidth(), theTextureMgr->getTexture("openingScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteRotAngle(), &spriteBkgd.getSpriteCentre());
	
		tempTextTexture = theTextureMgr->getTexture("MenuTitle");
		theTextureRectangle = tempTextTexture->getTextureRect();
		pos = { 380, 225, theTextureRectangle.w, theTextureRectangle.h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &theTextureRectangle, &pos);

		// Render Button
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 450, 300 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteRotAngle(), &theButtonMgr->getBtn("play_btn")->getSpriteCentre());
		theButtonMgr->getBtn("howtoplay_btn")->setSpritePos({ 450, 350 });
		theButtonMgr->getBtn("howtoplay_btn")->render(theRenderer, &theButtonMgr->getBtn("howtoplay_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("howtoplay_btn")->getSpritePos(), theButtonMgr->getBtn("howtoplay_btn")->getSpriteRotAngle(), &theButtonMgr->getBtn("howtoplay_btn")->getSpriteCentre());
		theButtonMgr->getBtn("quit_btn")->setSpritePos({ 450, 400 });
		theButtonMgr->getBtn("quit_btn")->render(theRenderer, &theButtonMgr->getBtn("quit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("quit_btn")->getSpritePos(), theButtonMgr->getBtn("quit_btn")->getSpriteRotAngle(), &theButtonMgr->getBtn("quit_btn")->getSpriteCentre());
	}
	break;
	case gameState::howtoplay: //'how to play' background and buttons
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("howToPlay"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("howToPlay")->getTWidth(), theTextureMgr->getTexture("howToPlay")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteRotAngle(), &spriteBkgd.getSpriteCentre());

		theButtonMgr->getBtn("play_btn")->setSpritePos({ 335 , 660 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteRotAngle(), &theButtonMgr->getBtn("play_btn")->getSpriteCentre());
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 435, 660 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteRotAngle(), &theButtonMgr->getBtn("menu_btn")->getSpriteCentre());
		theButtonMgr->getBtn("quit_btn")->setSpritePos({ 535, 660 });
		theButtonMgr->getBtn("quit_btn")->render(theRenderer, &theButtonMgr->getBtn("quit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("quit_btn")->getSpritePos(), theButtonMgr->getBtn("quit_btn")->getSpriteRotAngle(), &theButtonMgr->getBtn("quit_btn")->getSpriteCentre());
	}
	break;
	case gameState::playing: //background, collision border for fruits missed, sprite additions, and textures for health, score, basket, and food
	{

		spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteRotAngle(), &spriteBkgd.getSpriteCentre());


		border.render(theRenderer, &border.getSpriteDimensions(), &border.getSpritePos(), 0, &border.getSpriteCentre());
		border.setBoundingRect();

		if (fruitsCaught >= 5)
		{
			house.render(theRenderer, &house.getSpriteDimensions(), &house.getSpritePos(), 0, &house.getSpriteCentre());
		}
		if (fruitsCaught >= 10)
		{
			barn.render(theRenderer, &barn.getSpriteDimensions(), &barn.getSpritePos(), 0, &barn.getSpriteCentre());
		}
		if (fruitsCaught >= 15)
		{
			cow.render(theRenderer, &cow.getSpriteDimensions(), &cow.getSpritePos(), 0, &cow.getSpriteCentre());
		}
		if (fruitsCaught >= 17)
		{
			chicken.render(theRenderer, &chicken.getSpriteDimensions(), &chicken.getSpritePos(), 0, &chicken.getSpriteCentre());
			chicken2.render(theRenderer, &chicken2.getSpriteDimensions(), &chicken2.getSpritePos(), 0, &chicken2.getSpriteCentre());
		}
		if (fruitsCaught >= 18)
		{
			tree.render(theRenderer, &tree.getSpriteDimensions(), &tree.getSpritePos(), 0, &tree.getSpriteCentre());
			tree2.render(theRenderer, &tree2.getSpriteDimensions(), &tree2.getSpritePos(), 0, &tree2.getSpriteCentre());
		}
		if (fruitsCaught >= 19)
		{
			tractor.render(theRenderer, &tractor.getSpriteDimensions(), &tractor.getSpritePos(), 0, &tractor.getSpriteCentre());
		}

		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos);
		theTextureMgr->addTexture("health", theFontMgr->getFont("score")->createTextTexture(theRenderer, strHealth.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("health");
		pos = { 45, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos);

		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos);
		theTextureMgr->addTexture("score", theFontMgr->getFont("score")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("score");
		pos = { 815, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos);

		theCatcher.render(theRenderer, &theCatcher.getSpriteDimensions(), &theCatcher.getSpritePos(), theCatcher.getCatcherRotation(), &theCatcher.getSpriteCentre());
		theFruits.render(theRenderer);

	}
	break;
	case gameState::end: //game over background, text, and buttons
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("endScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("endScreen")->getTWidth(), theTextureMgr->getTexture("endScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteRotAngle(), &spriteBkgd.getSpriteCentre());

		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos);
		theTextureMgr->addTexture("gameOver", theFontMgr->getFont("score")->createTextTexture(theRenderer, strOver.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("gameOver");
		pos = { 325, 340, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos);


		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 350, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteRotAngle(), &theButtonMgr->getBtn("menu_btn")->getSpriteCentre());
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 450 , 500 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteRotAngle(), &theButtonMgr->getBtn("play_btn")->getSpriteCentre());
		theButtonMgr->getBtn("quit_btn")->setSpritePos({ 550, 500 });
		theButtonMgr->getBtn("quit_btn")->render(theRenderer, &theButtonMgr->getBtn("quit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("quit_btn")->getSpritePos(), theButtonMgr->getBtn("quit_btn")->getSpriteRotAngle(), &theButtonMgr->getBtn("quit_btn")->getSpriteCentre());
	}
	break;
	case gameState::quit: //quits app
	{
		loop = false;
	}
	break;
	default:
		break;
	}

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{
}

void cGame::update(double theDeltaTime)
{	
	//changes sound
	if (theGameState == gameState::menu && soundPlaying == false)
	{
		(theSoundMgr->getSnd("menu")->play(-1));
		soundPlaying = true;
	}
	else if (theGameState == gameState::playing && soundPlaying == false)
	{
		(theSoundMgr->getSnd("theme")->play(-1));
		soundPlaying = true;
	}

	switch (theGameState)
	{
	//menu buttons & gameStates
	case gameState::menu: 
	{
		theGameState = theButtonMgr->getBtn("quit_btn")->update(theGameState, gameState::quit, theAreaClicked);
		theGameState = theButtonMgr->getBtn("howtoplay_btn")->update(theGameState, gameState::howtoplay, theAreaClicked);
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);

		if (theButtonMgr->getBtn("play_btn")->getClicked()) 
		{
			theButtonMgr->getBtn("play_btn")->setClicked(false);
			soundPlaying = false;
		}
	}
	break;
	//'how to play' buttons & gameStates
	case gameState::howtoplay:
	{
		theGameState = theButtonMgr->getBtn("quit_btn")->update(theGameState, gameState::quit, theAreaClicked);
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);

		if (theButtonMgr->getBtn("play_btn")->getClicked())
		{
			soundPlaying = false;
		}
	}
	break;
	//updates fruits, basket, and counter for the speed of fruits
	case gameState::playing:
	{
		theFruits.update(theDeltaTime);
		theCatcher.update(theDeltaTime);

		counter++;

		if ((fruitsCaught >= 0 && fruitsCaught <= 5) && counter == 200)
		{
			theFruits.add(theTextureMgr, textureName, { (150 * (rand() % 6)) + 75, 0 }, 0.0f, 100, afruit++);
			counter = 0;
		}
		else if ((fruitsCaught > 4 && fruitsCaught <= 10) && counter == 150)
		{
			theFruits.add(theTextureMgr, textureName, { (150 * (rand() % 6)) + 75, 0 }, 0.0f, 100, afruit++);
			counter = 0;
		}
		else if ((fruitsCaught > 9 && fruitsCaught <= 15) && counter == 120)
		{
			theFruits.add(theTextureMgr, textureName, { (150 * (rand() % 6)) + 75, 0 }, 0.0f, 100, afruit++);
			counter = 0;
		}
		else if ((fruitsCaught > 14 && fruitsCaught <= 20) && counter == 100)
		{
			theFruits.add(theTextureMgr, textureName, { (150 * (rand() % 6)) + 75, 0 }, 0.0f, 100, afruit++);
			counter = 0;
		}
		
		//checks fruit collision with the basket
		if (theFruits.checkCollisions(&theCatcher.getBoundingRect(), "catcher"))
		{
			//creates and displays updated score
			if ((theFruits.theNumActiveFruits() + fruitsCaught + 1 + theFruits.getfruitsMissed()) == theFruits.theNumFruits())
			{
				fruitsCaught++;
				strScore = gameTextList2[0];
				strScore += to_string(fruitsCaught).c_str();
				theTextureMgr->deleteTexture("score");
			}
		}
		
		//checks fruits collision with border for fruits missed
		if (theFruits.checkCollisions(&border.getBoundingRect(), "border"))
		{ 
			//game over - lose condition
			if (theFruits.getfruitsMissed() == 3)
			{
				strOver = gameTextList2[2];
				strOver +=  to_string(fruitsCaught).c_str();
				theTextureMgr->deleteTexture("gameOver");

				theFruits.clear();
				fruitsCaught = theFruits.getFruitsCaught();
				afruit = 0;
				theFruits.setfruitsMissed(0);

				//resets fruits caught
				strScore = gameTextList2[0];
				strScore += to_string(fruitsCaught).c_str();
				theTextureMgr->deleteTexture("score");

				theGameState = gameState::end;
				
			}
			//resets fruits missed
			strHealth = gameTextList2[1];
			strHealth += to_string(theFruits.getfruitsMissed()).c_str();
			theTextureMgr->deleteTexture("health");
		}

		//game over - win condition
		if (theFruits.getFruitsCaught() == 20)
		{
			strOver = gameTextList2[2];
			strOver += to_string(fruitsCaught).c_str();
			theTextureMgr->deleteTexture("gameOver");

			theFruits.clear();
			fruitsCaught = theFruits.getFruitsCaught();
			afruit = 0;
			theFruits.setfruitsMissed(0);

			strScore = gameTextList2[0];
			strScore += to_string(fruitsCaught).c_str();

			//resets fruits missed
			strHealth = gameTextList2[1];
			strHealth += to_string(theFruits.getfruitsMissed()).c_str();
			theTextureMgr->deleteTexture("health");

			theGameState = gameState::end;

		}
	}
	break;
	//buttons & resets area clicked
	case gameState::end:
	{
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
		theGameState = theButtonMgr->getBtn("quit_btn")->update(theGameState, gameState::quit, theAreaClicked);

		if (theButtonMgr->getBtn("play_btn")->getClicked())
		{
			theAreaClicked = { 0,0 };
			soundPlaying = false;
		}
		if (theButtonMgr->getBtn("menu_btn")->getClicked() && soundPlaying == true)
		{
			theAreaClicked = { 0,0 };
			soundPlaying = false;
		}
	}
	break;
	//quits
	case gameState::quit:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			{
			}
			break;
			case SDL_KEYDOWN:
				
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
					
				}
				break;

				case SDLK_RIGHT:
				{
					//basket position to right
					if (theCatcher.getSpritePos().x < 1024 - theCatcher.getSpriteDimensions().w)
					{
						theCatcher.setSpritePos({ theCatcher.getSpritePos().x + 10, theCatcher.getSpritePos().y });
					}
				}
				break;

				case SDLK_LEFT:
				{
					//basket position to left
					if (theCatcher.getSpritePos().x > 0)
					{
						theCatcher.setSpritePos({ theCatcher.getSpritePos().x - 10, theCatcher.getSpritePos().y });
					}
				}
				break;
				case SDLK_UP:
				{
					
				}
				break;
				case SDLK_d:
				{
					
				}
				break;

				case SDLK_a:
				{
					
				}
				break;
				case SDLK_w:
				{
					
				}
				break;

				case SDLK_s:
				{
					
				}
				break;

				case SDLK_SPACE:
				{
				}
				break;
				default:
					break;
				}
			case SDL_TEXTINPUT:
			{
				//cin.clear();
			//	if (event.key.keysym.sym == SDLK_BACKSPACE && userName.length() > 0)
			//	{
			//		// Remove last character
			//		userName.pop_back();
			//		renderText = true;
			//		cout << userName << endl;
			//	}
			//	else if (event.key.keysym.sym == SDLK_RETURN && userName.size() != 0)
			//	{
			//		//inputCmd = inputText.c_str();
			//		renderText = true;
			//		//inputText = "";
			//	}
			//	else
			//	{
			//		userName += (char)(event.key.keysym.sym); //event.text.text[0];
			//		renderText = true;
			//	}
			//	
			//	cout << userName << endl;
			}
			break;
			case SDL_TEXTEDITING:
			{

			}
			break;
			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{

	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	//Quit FONT system
	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

