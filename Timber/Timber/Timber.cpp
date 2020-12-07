#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

// Function Declarations
void updateBranches(int seed);

// Global variables
const int NUM_BRANCHES = 6;
Sprite spriteBranches[NUM_BRANCHES];
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

side playerSide = side::LEFT;
const float AXE_POSITION_LEFT = 700;
const float AXE_POSITION_RIGHT = 1075;


int main()
{
	VideoMode vm(1920, 1080);

	RenderWindow window(vm, "Timber", Style::Fullscreen);

	//Textures
	Texture textureBackground;
	Texture textureTree;
	Texture textureBee;
	Texture textureCloud;
	Texture textureBranch;
	Texture texturePlayer;
	Texture textureRIP;
	Texture textureAxe;
	Texture textureLog;

	textureBackground.loadFromFile("graphics/background.png");
	textureTree.loadFromFile("graphics/tree.png");
	textureBee.loadFromFile("graphics/bee.png");
	textureCloud.loadFromFile("graphics/cloud.png");
	textureBranch.loadFromFile("graphics/branch.png");
	texturePlayer.loadFromFile("graphics/player.png");
	textureRIP.loadFromFile("graphics/rip.png");
	textureAxe.loadFromFile("graphics/axe.png");
	textureLog.loadFromFile("graphics/log.png");

	// Sprites
	Sprite spriteBackground;
	Sprite spriteTree;
	Sprite spriteBee;
	Sprite spritePlayer;
	Sprite spriteRIP;
	Sprite spriteAxe;
	Sprite spriteLog;

	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0); //png is 1920x1080 so it will fill the entire screen at 0,0
	
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	spriteCloud1.setTexture(textureCloud);
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud3.setPosition(0, 500);

	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		spriteBranches[i].setTexture(textureBranch);
		spriteBranches[i].setPosition(-2000, -2000); // Hides off screen for now

		spriteBranches[i].setOrigin(220, 20); // sets origin to center of itself (rather than top left corner)
	}

	// Text
	int score = 0;
	sf::Text messageText;
	sf::Text scoreText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);

	messageText.setString("Press Enter To Start!");
	scoreText.setString("Score = 0");

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	FloatRect textRect = messageText.getLocalBounds(); // rectangle of float coords
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	// HUD
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Sound
	SoundBuffer chopBuffer;
	SoundBuffer deathBuffer;
	SoundBuffer outOfTimeBuffer;

	Sound chopSound;
	Sound deathSound;
	Sound outOfTimeSound;

	chopBuffer.loadFromFile("sound/chop.wav");
	deathBuffer.loadFromFile("sound/death.wav");
	outOfTimeBuffer.loadFromFile("sound/out_of_time.wav");

	chopSound.setBuffer(chopBuffer);
	deathSound.setBuffer(deathBuffer);
	outOfTimeSound.setBuffer(outOfTimeBuffer);






	// Variables
	Clock clock;
	
	bool beeActive = false;
	float beeSpeed = 0.0f;

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	bool paused = true;

	bool acceptInput = false;

	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);
	
	// Main Loop
	while (window.isOpen())
	{
		// INPUT
		
		// Reset after swing
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		
		// Exit game
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Unpause/Start game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			timeRemaining = 5;

			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			spriteRIP.setPosition(675, 2000);

			spritePlayer.setPosition(580, 720);
			
			acceptInput = true;

		}

		// Player control input
		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerSide = side::RIGHT;
				score++;
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				chopSound.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerSide = side::LEFT;
				score++;
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);

				updateBranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				chopSound.play();
			}
		}

		
		// UPDATE
		if (!paused)
		{
			Time dt = clock.restart(); //restarts clock every frame AND stores the time from last frame as DeltaTime

			//Update Time Bar
			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				paused = true;

				outOfTimeSound.play();

				messageText.setString("Out of Time!");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

			}

			// Setup inactive Bee
			if (!beeActive)
			{
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true; // initial bee setup, only runs if bee is inactive
			}
			// Move an active Bee (reset if at end of screen)
			else
			{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}

			// Setup inactive Cloud1
			if (!cloud1Active)
			{
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200) + 100;

				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			// Move an active Bee (reset if at end of screen)
			else
			{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);

				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}

			// Setup inactive Cloud2
			if (!cloud2Active)
			{
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200) + 100;

				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			// Move an active Bee (reset if at end of screen)
			else
			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);

				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			// Setup inactive Cloud3
			if (!cloud3Active)
			{
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200) + 100;

				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			// Move an active Bee (reset if at end of screen)
			else
			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);

				if (spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}

			// Update Score
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// Update branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					spriteBranches[i].setPosition(610, height);
					spriteBranches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					spriteBranches[i].setPosition(1330, height);
					spriteBranches[i].setRotation(0);
				}
				else
				{
					spriteBranches[i].setPosition(3000, height); // Hides the branch
				}

			}

			// Handle Logs flying
			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// Player getting smashed (no physics detection, use coordinates for Megaman similar to this)
			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				spriteRIP.setPosition(spritePlayer.getPosition().x, spritePlayer.getPosition().y);
				spritePlayer.setPosition(2000, 660);
				
				deathSound.play();
				
				messageText.setString("SMASHED!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}
		}

		
		// DRAW
		window.clear();

		window.draw(spriteBackground); // Simple drawing must be done in order or things will be covered

		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(spriteBranches[i]);
		}

		window.draw(spriteTree);

		window.draw(spritePlayer);

		window.draw(spriteAxe);
		
		window.draw(spriteLog);
		
		window.draw(spriteRIP);

		window.draw(spriteBee);

		window.draw(scoreText);

		window.draw(timeBar);

		if (paused)
		{
			window.draw(messageText);
		}

		window.display();
	}
	return 0;
}

void updateBranches(int seed)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	//spawn new branch
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}
