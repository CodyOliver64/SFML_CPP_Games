#include <iostream>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"


using namespace sf;

int main()
{
	TextureHolder texHolder;

	enum class State
	{
		PAUSED, LEVELING_UP, GAME_OVER, PLAYING
	};

	State state = State::GAME_OVER;

	// Create Game Window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Default);

	// Camera
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Game Clock
	Clock clock;
	Time gameTimeTotal;

	// Mouse Coords (world/screen)
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	// Game Objects
	Player player;
	IntRect arena;

	VertexArray background;
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	Bullet bullets[100];
	Time lastPressed;
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;

	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// Add Crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Stats
	int score = 0;
	int hiScore = 0;

	
	// HUD AND TEXT 
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 950);

	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(550, 400);
	pausedText.setString("Press Enter \nto continue");

	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");

	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream << "1- Increased rate of fire" << "\n2- Increased clip size(next reload)" << "\n3- Increased max health" << "\n4- Increased run speed" << "\n5- More and better health pickups" << "\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 950);

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	// Read hi score from file
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 950);
	zombiesRemainingText.setString("Zombies: 100");

	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 950);
	waveNumberText.setString("Wave: 0");

	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 945);

	// HUD Control
	int framesSinceLastHUDUpdate = 0;
	int fpsMeasurementFrameInterval = 75;

	// Sound
	SoundBuffer hitBuffer;
	SoundBuffer splatBuffer;
	SoundBuffer shootBuffer;
	SoundBuffer reloadBuffer;
	SoundBuffer reloadFailedBuffer;
	SoundBuffer powerupBuffer;
	SoundBuffer pickupBuffer;

	Sound hit;
	Sound splat;
	Sound shoot;
	Sound reload;
	Sound reloadFailed;
	Sound powerup;
	Sound pickup;

	hitBuffer.loadFromFile("sound/hit.wav");
	splatBuffer.loadFromFile("sound/splat.wav");
	shootBuffer.loadFromFile("sound/shoot.wav");
	reloadBuffer.loadFromFile("sound/reload.wav");
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	powerupBuffer.loadFromFile("sound/powerup.wav");
	pickupBuffer.loadFromFile("sound/pickup.wav");

	hit.setBuffer(hitBuffer);
	splat.setBuffer(splatBuffer);;
	shoot.setBuffer(shootBuffer);;
	reload.setBuffer(reloadBuffer);;
	reloadFailed.setBuffer(reloadFailedBuffer);;
	powerup.setBuffer(powerupBuffer);;
	pickup.setBuffer(pickupBuffer);;





	// Main Loop
	while (window.isOpen())
	{
		/*
		----------------
		HANDLE INPUT
		----------------
		*/

		// Events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					clock.restart(); // prevents frame jump after unpausing
				}
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;

					wave = 0;
					score = 0;
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;
					player.resetPlayerStats();
				}

				if (state == State::PLAYING)
				{
					if (event.key.code == Keyboard::R)
					{
						// Enough bullets to fill clip
						if (bulletsSpare >= clipSize)
						{
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							reload.play();
						}
						// Partially fill clip
						else if (bulletsSpare > 0)
						{
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						// No bullets left to reload
						else
						{
							reloadFailed.play();
						}
					}
				}
			}
		}

		// Player Input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (state == State::PLAYING)
		{
			// Move Up
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}
			// Move Down
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}
			// Move Right
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			// Move Left
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			// Fire bullet
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;

					if (currentBullet > 99)
					{
						currentBullet = 0;
					}

					lastPressed = gameTimeTotal;

					shoot.play();
					
					bulletsInClip--;
				}
			}
		}

		if (state == State::LEVELING_UP)
		{
			if (event.key.code == Keyboard::Num1)
			{
				fireRate++;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num2)
			{
				clipSize += clipSize;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3)
			{
				player.upgradeHealth();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4)
			{
				player.upgradeSpeed();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num5)
			{
				healthPickup.upgrade();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num6)
			{
				ammoPickup.upgrade();
				state = State::PLAYING;
			}

			// setup game after levelup
			if (state == State::PLAYING)
			{
				wave++;

				arena.width = 500 * wave;
				arena.height = 500 * wave;
				arena.left = 0;
				arena.top = 0;
				
				int tileSize = createBackground(background, arena); // fills vertex array using sprite sheet texture

				player.spawn(arena, resolution, tileSize);

				// Create Pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);


				// Create zombie horde
				numZombies = 5 * wave;
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				powerup.play();

				clock.restart();
			}
		}

		/*
		----------------
		 UPDATE FRAME
		----------------
		*/

		if (state == State::PLAYING)
		{
			// Update delta
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			// Get mouse pos
			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			// Set crosshair
			spriteCrosshair.setPosition(mouseWorldPosition);

			// Update player and save position
			player.update(dtAsSeconds, Mouse::getPosition());
			Vector2f playerPosition(player.getCenter());

			// Update camera
			mainView.setCenter(player.getCenter());

			// Update Zombies
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			// Update bullets
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}

			// Update pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			// ********* BULLET COLLISION DETECTION **********
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					// current bullet/zombie is flying/alive?
					if (bullets[i].isInFlight() && zombies[j].isAlive())
					{
						// bullet collides with zombie?
						if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
						{
							bullets[i].stop();

							// register zombie hit (calls hit which returns true if it dies)
							if (zombies[j].hit())
							{
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}

								numZombiesAlive--;

								// level up if you kill all zombies
								if (numZombiesAlive == 0)
									state = State::LEVELING_UP;
							}
							splat.play();
						}
					}
				}
			}
			// ********* ZOMBIE/PLAYER COLLISION DETECTION **********
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					// proccess player hit
					if (player.hit(gameTimeTotal))
					{
						hit.play();
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			}
			// ********* PICKUP/PLAYER COLLISION DETECTION **********
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				pickup.play();
			}
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
				reload.play();
			}

			// Update HUD
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			framesSinceLastHUDUpdate++;

			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{

				// Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;
			}
		}

		/*
		----------------
		  DRAW SCENE
		----------------
		*/

		if (state == State::PLAYING)
		{
			window.clear();

			window.setView(mainView);

			window.draw(background, &textureBackground);

			// Draw zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}

			// Draw bullets
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			// draw player
			window.draw(player.getSprite());

			// Draw pickups
			if (healthPickup.isSpawned())
				window.draw(healthPickup.getSprite());
			if (ammoPickup.isSpawned())
				window.draw(ammoPickup.getSprite());

			window.draw(spriteCrosshair);

			// Draw HUD
			window.setView(hudView);

			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}
		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		window.display();
	}
	
	delete[] zombies; // frees memory
	return 0;
}
