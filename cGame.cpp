/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

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
	textureName = { "theBackground", "apple", "carrot" , "lemon" , "raspberry" , "strawberry" , "basket" };
	texturesToUse = { "Images/Bkg/FarmyardBkg.png", "Images/Sprites/Apple.png", "Images/Sprites/Carrot.png" , "Images/Sprites/Lemon.png" , "Images/Sprites/Raspberry.png" , "Images/Sprites/Strawberry.png" , "Images/Sprites/Basket64x64.png" };
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	aColour = { 228, 213, 238, 255 };
	// Create textures for Game Dialogue (text)
	fontList = { "fruit", "strawberry" };
	fontsToUse = { "Fonts/FarmToMarketFancy-n8P1.ttf", "Fonts/Strawberry-Regular.ttf" };
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
	gameTextNames = { "TitleTxt"};
	gameTextList = { "Fruit Fall"};
	for (unsigned int text = 0; text < gameTextNames.size(); text++)
	{
		if (text == 0 || text == gameTextNames.size()-1)
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("fruit")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		}
		else
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("strawberry")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		}
	}
	// Load game sounds
	soundList = { "theme", "click" };
	soundTypes = { soundType::music, soundType::sfx};
	soundsToUse = { "Audio/Theme/Kevin_MacLeod_-_Master_of_the_Feast.mp3", "Audio/SFX/ClickOn.wav"};
	for (unsigned int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());


	theCatcher.setCatcherPos({ 500, 600 });
	theCatcher.setTexture(theTextureMgr->getTexture("basket"));
	theCatcher.setSpriteDimensions(theTextureMgr->getTexture("basket")->getTWidth(), theTextureMgr->getTexture("basket")->getTHeight());

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

	// 
	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteRotAngle(), &spriteBkgd.getSpriteCentre());

	tempTextTexture = theTextureMgr->getTexture("TitleTxt");
	theTextureRectangle = tempTextTexture->getTextureRect();
	pos = { 10, 10, theTextureRectangle.w, theTextureRectangle.h };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &theTextureRectangle, &pos);

	theCatcher.render(theRenderer, &theCatcher.getSpriteDimensions(), &theCatcher.getSpritePos(), theCatcher.getCatcherRotation(), &theCatcher.getSpriteCentre());

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{
}

void cGame::update(double theDeltaTime)
{
	theCatcher.update(theDeltaTime);
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
					
				}
				break;

				case SDLK_LEFT:
				{
					
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

