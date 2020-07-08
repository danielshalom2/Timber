// helloSfml.cpp : This file contains the 'main' function. Program execution begins and ends there.

//#include "stdafx.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side
{
	LEFT, RIGHT, NONE
};
side branchPositions[NUM_BRANCHES];

int main()
{
	//create video mode object
	VideoMode vm(1920, 1080);
	//create and open window for the game.
	RenderWindow window(vm, "Timber!!", Style::Fullscreen);
	// create texture
	Texture textureBackground;
	//load graphic
	textureBackground.loadFromFile("graphics/background.png");
	//create sprite
	Sprite spriteBackground;
	//attach the texture
	spriteBackground.setTexture(textureBackground);
	//set the spritebackground to cover the screen
	spriteBackground.setPosition(0, 0);
	//make tree
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(350, -50);
	//make bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	//Is the bee currently moving?
	bool beeActive = false;
	float beeSpeed = 0.0f;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer out_of_timeBuffer;
	out_of_timeBuffer.loadFromFile("sound/out_of_time.wav");
	Sound out_of_time;
	out_of_time.setBuffer(out_of_timeBuffer);

	//Clouds
	Texture textureCloud;
	//Load texture
	textureCloud.loadFromFile("graphics/cloud.png");
	//3 clouds
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 150);
	spriteCloud3.setPosition(0, 300);
	//cloud move
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	//the velocity of cloud (how fast)
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//variables to control time itself
	Clock clock;

	//time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 800;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Cyan);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	std::ifstream highscoreFile("highscore.txt");

	//the game is stopped?
	bool paused = true;
	//draw text
	int score = 0;
	int highscore;
	if (highscoreFile.is_open())
	{
		while (!highscoreFile.eof())
			highscoreFile >> highscore;
	}
	else
		highscore = 0;
	sf::Text messageText;
	sf::Text scoreText;
	sf::Text highscoreText;

	//choose font
	Font font;
	font.loadFromFile("font/ZOMBIES REBORN.ttf");
	//set font
	messageText.setFont(font);
	scoreText.setFont(font);
	highscoreText.setFont(font);
	//assign massage
	messageText.setString("Press Enter to Start!");
	scoreText.setString("Score = 0");
	highscoreText.setString("Highscore = ");
	//set size of text
	messageText.setCharacterSize(200);
	scoreText.setCharacterSize(100);
	highscoreText.setCharacterSize(50);
	//color
	messageText.setFillColor(Color::Red);
	scoreText.setFillColor(Color::Red);
	highscoreText.setFillColor(Color::Red);
	//position of text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);
	highscoreText.setPosition(20, 120);

	//prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	//set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		//set the origin to dead center
		//spin it round withoud change position
		branches[i].setOrigin(270, 50);
	}
	//prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);
	//the player start on the left
	side playerSide = side::LEFT;
	//preapare gravestone
	Texture textureRip;
	textureRip.loadFromFile("graphics/rip.png");
	Sprite spriteRip;
	spriteRip.setTexture(textureRip);
	spriteRip.setPosition(600, 860);

	//prepare axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(750, 830);

	//line the axe up with the tree
	const float AXE_POSITION_LEFT = 750;
	const float AXE_POSITION_RIGHT = 1180;

	//Prepare the log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(840, 720);
	//some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);


	while (window.isOpen())
	{
		/*
		****************************
		handle the players input
		***********************
		*/
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				//listen for key press againg
				acceptInput = true;
				//hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			out_of_time.stop();
			death.stop();
			timeRemaining = 5;
			//make all branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}
			//make sure the gravestone is hidden
			spriteRip.setPosition(675, 2000);
			//move player into position
			spritePlayer.setPosition(580, 720);
			if (spriteAxe.getScale().x == -1.f && spriteAxe.getScale().y == 1.f)
			{
				spriteAxe.scale(-1.f, 1.f);
			}
			spriteAxe.setPosition(750, 830);
			acceptInput = true;
		}
		//wrap the player control to make sure we accept input
		if (acceptInput)
		{
			//here code
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				if (spriteAxe.getScale().x != -1.f || spriteAxe.getScale().y != 1.f)
				{
					spriteAxe.scale(-1.f, 1.f);
				}
				playerSide = side::RIGHT;
				score++;
				chop.play();
				//add to the amount of time remain
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1150, 720);
				//update brances
				updateBranches(score);
				//set the flying log to the left
				spriteLog.setPosition(840, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				if (spriteAxe.getScale().x == -1.f && spriteAxe.getScale().y == 1.f)
				{
					spriteAxe.scale(-1.f, 1.f);
				}
				playerSide = side::LEFT;
				score++;
				chop.play();
				//add to the amount of time remain
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);
				//update brances
				updateBranches(score);
				//set the flying log to the left
				spriteLog.setPosition(840, 720);
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false;
			}
		}
		//start the game:

		/*
		****************************
		update the scene.
		***********************
		*/
		//measure time
		if (!paused)
		{
			Time dt = clock.restart();
			//subtract from time remain
			timeRemaining -= dt.asSeconds();
			//size up time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
			if (timeRemaining <= 0.0f)
			{
				//pause the game
				paused = true;
				//change the massage to the [player
				messageText.setString("out of time!!!");
				out_of_time.play();
				//reposition the text
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			}



			//setup the bee
			if (!beeActive)
			{
				//how fast is the bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 350;
				//how high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else//move the bee

			{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
				//has the bee reached the right hand edge?
				if (spriteBee.getPosition().x < -100)
				{
					//set it up ready to be whole new cloud next frame
					beeActive = false;
				}
			}

			//CLOUDS
			if (!cloud1Active)
			{
				//how fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);
				//how high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}

			else//move the cloud

			{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
				//has the bee reached the right hand edge?
				if (spriteCloud1.getPosition().x > 1920)
				{
					//set it up ready to be whole new cloud next frame
					cloud1Active = false;
				}
			}

			if (!cloud2Active)
			{
				//how fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);
				//how high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}

			else//move the cloud

			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
				//has the bee reached the right hand edge?
				if (spriteCloud2.getPosition().x > 1920)
				{
					//set it up ready to be whole new cloud next frame
					cloud2Active = false;
				}
			}
			if (!cloud3Active)
			{
				//how fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);
				//how high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}

			else//move the cloud

			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
				//has the bee reached the right hand edge?
				if (spriteCloud3.getPosition().x > 1920)
				{
					//set it up ready to be whole new cloud next frame
					cloud3Active = false;
				}
			}
			//update score
			std::stringstream ss;
			std::stringstream hs;
			ss << "Score = " << score;

			std::ofstream output("highscore.txt");
			if (score > highscore)
			{
				highscore = score;
				output << score;
			}
			else
			{
				output << highscore;
			}
			hs << "HighScore = " << highscore;
			highscoreText.setString(hs.str());
			scoreText.setString(ss.str());



			//update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					//move the sprite to left side
					branches[i].setPosition(910, height);

					//rotate
					//branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					//move to right side
					branches[i].setPosition(1330, height);
					//rotate
					branches[i].setRotation(0);
				}
				else//hide the branch
					branches[i].setPosition(3000, height);
			}
			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x
					+ (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y
					+ (logSpeedY * dt.asSeconds()));
				//has log reach right?
				if (spriteLog.getPosition().x <= -100 ||
					spriteLog.getPosition().x > 2000)
				{
					//set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(840, 720);


				}
			}
			if (branchPositions[5] == playerSide)
			{
				//death
				paused = true;
				acceptInput = false;
				death.play();
				//draw gravestione
				spriteRip.setPosition(525, 760);
				//hide the player
				spritePlayer.setPosition(3000, 660);
				//hide the axe
				spriteAxe.setPosition(3000, 660);
				spriteLog.setPosition(840, 720);
				//change text of message

				messageText.setString("SQUISHED!!!!!");
				//center it on screen
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080. / 2.0f);
			}

		}//enf if(!paused)


		/*
		****************************
		draw the scene.
		***********************
		*/
		//clear everything from last frame
		window.clear();

		// draw game scene
		window.draw(spriteBackground);
		//draw the cloud
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		//draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++)
			window.draw(branches[i]);
		//draw the log
		window.draw(spriteLog);
		//draw the tree
		window.draw(spriteTree);
		//draw the Player
		window.draw(spritePlayer);
		//draw the axe
		window.draw(spriteAxe);

		//draw the gravestone
		window.draw(spriteRip);
		//draw the BEE
		window.draw(spriteBee);
		//draw score
		window.draw(scoreText);
		window.draw(highscoreText);
		window.draw(timeBar);
		if (paused)
			window.draw(messageText);
		// show everything we drew
		window.display();

	}
	return 0;
}


void updateBranches(int seed)
{
	//move all branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
		branchPositions[j] = branchPositions[j - 1];
	//initilize new branch at position 0 (LEFT RIGHT OR NONE)
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