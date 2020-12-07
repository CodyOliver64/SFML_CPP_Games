#pragma once

#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "Thomas.h"
#include "Bob.h"
#include "LevelManager.h"
#include "SoundManager.h"
#include "HUD.h"



using namespace sf;

class Engine
{
private:
	TextureHolder th;

	// Player characters
	Thomas m_Thomas;
	Bob m_Bob;

	LevelManager m_LM;

	SoundManager m_SM;

	HUD m_HUD;
	int m_FramesSinceLastHUDUpdate = 0;
	int m_TargetFramesPerHUDUpdate = 500;

	const int TILE_SIZE = 50;
	const int VERTS_IN_QUAD = 4;
	const int GRAVITY = 300;

	RenderWindow m_Window;

	// Main views
	View m_MainView;
	View m_LeftView;
	View m_RightView;

	// Background views
	View m_BGMainView;
	View m_BGLeftView;
	View m_BGRightView;

	View m_HudView;

	Sprite m_BackgroundSprite;
	Texture m_BackgroundTexture;


	bool m_Playing = false;
	bool m_SplitScreen = false;
	bool m_Character1 = true;

	float m_TimeRemaining;
	Time m_GameTimeTotal;

	bool m_NewLevelRequired = true;

	// Level variables
	VertexArray m_VALevel;
	int** m_ArrayLevel = NULL;
	Texture m_TextureTiles;

	vector <Vector2f> m_FireEmitters;
	
	void input();
	void update(float dtAsSeconds);
	void draw();

	void loadLevel();

	bool detectCollisions(PlayableCharacter& character);

	void populateEmitters(vector <Vector2f>& vSoundEmitters, int** arrayLevel);



public:
	Engine();
	void run();

};