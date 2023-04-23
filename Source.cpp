#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

const int  window_width = 720;
const int window_height = 1080;

RenderWindow window(VideoMode(window_width, window_height), "Flappy Bird", Style::Default);

struct Bird
{
	Texture texture;
	Sprite sprite;

}FlappyBird;

struct Textures
{
	Texture background, startButton, title, ground, gameover, pause, pausemenu, upperpipe, lowerpipe, getready, scoreboard, coin;

}texture;

struct Sprites
{

	Sprite background, startButton, title, ground, gameover, pause, pausemenu, getready, scoreboard, upperpipe[7], lowerpipe[7], coin[7];

}sprite;

Text scoreTxt, highscoreTxt;

void TexturesApplied()
{
	FlappyBird.texture.loadFromFile("Flappy animation.png");
	texture.background.loadFromFile("Flappy Background.png");
	texture.startButton.loadFromFile("Flappy start.png");
	texture.title.loadFromFile("Flappy Title.png");
	texture.ground.loadFromFile("Flappy Ground.png");
	texture.gameover.loadFromFile("Flappy Game Over.png");
	texture.pause.loadFromFile("Flappy Pause.png");
	texture.pausemenu.loadFromFile("Flappy PauseMenu.png");
	texture.upperpipe.loadFromFile("Flappy Upper pipe.png");
	texture.lowerpipe.loadFromFile("Flappy lower pipe.png");
	texture.getready.loadFromFile("Flappy Get Ready.png");
	texture.scoreboard.loadFromFile("Flappy score board.png");
	texture.coin.loadFromFile("coin.png");
}

void setup(double j, double yPos[], double yscale[], double coinPos[])
{
	FlappyBird.sprite.setTexture(FlappyBird.texture);
	FlappyBird.sprite.setScale(4, 4);
	FlappyBird.sprite.setColor(Color::Transparent);
	FlappyBird.sprite.setPosition(320, 530);

	sprite.background.setTexture(texture.background);
	sprite.background.setOrigin(0, 0);
	sprite.background.setScale(5, 4.25);

	sprite.startButton.setTexture(texture.startButton);
	sprite.startButton.setPosition(233, 485);
	sprite.startButton.setScale(4, 4);

	sprite.title.setTexture(texture.title);
	sprite.title.setScale(5, 4.5);
	sprite.title.setPosition(124, 180);

	sprite.ground.setTexture(texture.ground);
	sprite.ground.setScale(4.5, 5);
	sprite.ground.setPosition(0, 850);
	sprite.ground.setColor(Color::Transparent);

	sprite.gameover.setTexture(texture.gameover);
	sprite.gameover.setPosition(120, 180);
	sprite.gameover.setScale(5, 4.5);
	sprite.gameover.setColor(Color::Transparent);

	sprite.getready.setTexture(texture.getready);
	sprite.getready.setColor(Color::Transparent);
	sprite.getready.setScale(5, 4.5);
	sprite.getready.setPosition(120, 180);

	sprite.pause.setTexture(texture.pause);
	sprite.pause.setPosition(0, 0);
	sprite.pause.setScale(0, 0);

	sprite.pausemenu.setTexture(texture.pausemenu);
	sprite.pausemenu.setScale(0, 0);
	sprite.pausemenu.setPosition(250, 400);

	sprite.scoreboard.setTexture(texture.scoreboard);
	sprite.scoreboard.setPosition(250, 350);
	sprite.scoreboard.setScale(0, 0);

	for (int i = 0; i < 7; i++)
	{
		sprite.upperpipe[i].setTexture(texture.upperpipe);
		sprite.upperpipe[i].setScale(Vector2f(0, 0));
		sprite.upperpipe[i].setPosition(Vector2f(j + 720.0, 0.0));

		sprite.lowerpipe[i].setTexture(texture.lowerpipe);
		sprite.lowerpipe[i].setScale(Vector2f(0, 0));
		sprite.lowerpipe[i].setPosition(Vector2f(j + 720.0, yPos[i]));

		sprite.coin[i].setTexture(texture.coin);
		sprite.coin[i].setScale(Vector2f(0, 0));
		sprite.coin[i].setColor(Color::Transparent);
		sprite.coin[i].setPosition(Vector2f(j + 700.0, coinPos[i]));

		j += 300.0; // distance between each pipe and coins
	}
}

void Birdmotionup(int jumpspeed, Vector2f& velocity)
{
	velocity.y = -jumpspeed - 8.00;
}
void Birdmotiondown(int jumpspeed, Vector2f& velocity)
{
	velocity.y = jumpspeed + 6.0;
}

void Pipesmotion(Vector2f& velocityPipe, float mpipe, double accPipes)
{
	velocityPipe.x += mpipe - accPipes;
}

bool collision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2, float x3, float y3, float w3, float h3)
{
	if (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 || x1 + w1 >= x3 && x1 <= x3 + w3 && y1 <= y3 + h3)
	{
		return true;
	}
	return false;
}

void highscore(int score, int& hScore)
{
	if (score > hScore)
	{
		hScore = score;
	}
}

void Draw()
{
	window.draw(sprite.background);
	window.draw(sprite.startButton);
	window.draw(sprite.title);

	for (int i = 0; i < 7; i++)

	{
		window.draw(sprite.upperpipe[i]);
		window.draw(sprite.lowerpipe[i]);
		window.draw(sprite.coin[i]);
	}

	window.draw(sprite.getready);
	window.draw(sprite.pause);
	window.draw(sprite.gameover);
	window.draw(FlappyBird.sprite);
	window.draw(sprite.scoreboard);
	window.draw(scoreTxt);
	window.draw(highscoreTxt);
	window.draw(sprite.pausemenu);
	window.draw(sprite.ground);
}

void main()
{
	window.setFramerateLimit(30);

	bool pause = true, collides = false;

	double yScale[] = { 2.0, 1.0, 1.0, 2.25, 3.0, 1.5, 2.5 };   //upper pipe scale
	double yPos[] = { 500.0, 300.0, 300.0 , 550.0, 650.0, 400.0, 600.0 };   // lower pipe position
	double coinPos[] = { 350.0,150.0,150.0,400.0,500.0,250.0,450.0 }; //coins position
	double j = 0.0;

	int groundheight = 800;
	Vector2f velocity(Vector2f(0.f, 0.f));
	float jumpspeed = 1.50f;

	Vector2f velocityPipe(Vector2f(0, 0));
	const float mpipe = 3.0f;

	int score = 0, hScore = -1;

	float Fx, Fy, Fw, Fh, Lx, Ly, Lw, Lh, Ux, Uy, Uw, Uh;

	double accPipes = 2.5;

	struct Buffers
	{

		SoundBuffer flapBuffer, scoringBuffer, collisionBuffer;

	}Buffers;

	Buffers.flapBuffer.loadFromFile("flap-sound.wav");
	Buffers.scoringBuffer.loadFromFile("scoring-sound.wav");
	Buffers.collisionBuffer.loadFromFile("Collision-sound.wav");

	struct Sounds
	{
		Sound flap, scoring, collision;

	}Sounds;

	Sounds.flap.setBuffer(Buffers.flapBuffer);
	Sounds.scoring.setBuffer(Buffers.scoringBuffer);
	Sounds.collision.setBuffer(Buffers.collisionBuffer);

	Font scoreFont;

	scoreFont.loadFromFile("Flappy txt.ttf");

	scoreTxt.setFillColor(Color::Transparent);
	scoreTxt.setFont(scoreFont);
	scoreTxt.setCharacterSize(35);

	highscoreTxt.setFillColor(Color::Transparent);
	highscoreTxt.setFont(scoreFont);
	highscoreTxt.setCharacterSize(35);

	TexturesApplied(); //Applying textures Function Call

	setup(j, yPos, yScale, coinPos); //setting up sprites Function Call

	Clock startClock, accClock;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			Vector2i mousePosition = Mouse::getPosition(window);

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (mousePosition.x > 233 && mousePosition.x < 484 && mousePosition.y > 501 && mousePosition.y < 605)
				{
					sprite.startButton.setColor(Color::Transparent);
					sprite.title.setColor(Color::Transparent);

					FlappyBird.sprite.setColor(Color::White);
					FlappyBird.sprite.setTextureRect(IntRect(0, 24, 20, 24));

					sprite.ground.setColor(Color::White);
					sprite.getready.setColor(Color::White);
					accClock.restart();

					pause = false;

					for (int i = 0; i < 7; i++)
					{
						sprite.upperpipe[i].setScale(Vector2f(3, yScale[i]));
						sprite.lowerpipe[i].setScale(Vector2f(3, 4));
						sprite.coin[i].setScale(Vector2f(0.22, 0.22));
					}
					startClock.restart().asSeconds();
				}
			}

			//Pause Menu

			if (startClock.getElapsedTime().asSeconds() > 3)
			{
				if (Keyboard::isKeyPressed(Keyboard::Escape))
				{
					sprite.pausemenu.setScale(5, 5);
					pause = true;
				}
				if (pause == true)
				{
					sprite.scoreboard.setScale(0, 0);
					scoreTxt.setFillColor(Color::Transparent);
					highscoreTxt.setFillColor(Color::Transparent);
					sprite.gameover.setColor(Color::Transparent);
					Vector2i mousePosition2 = Mouse::getPosition(window);

					if (Mouse::isButtonPressed(Mouse::Left))
					{
						if (mousePosition2.x > 250 && mousePosition2.x < 450 && mousePosition2.y > 400 && mousePosition2.y < 460)  //Menu
						{

							sprite.pause.setScale(0, 0);
							sprite.pausemenu.setScale(0, 0);
							sprite.startButton.setColor(Color::White);
							sprite.title.setColor(Color::White);
							FlappyBird.sprite.setColor(Color::Transparent);
							sprite.ground.setColor(Color::Transparent);
							FlappyBird.sprite.setPosition(320, 530);

							accClock.restart();
							accPipes = 2.5;
							jumpspeed = 1.50f;
							score = 0;

							collides = false;

							j = 0;

							for (int i = 0; i < 7; i++)

							{

								sprite.upperpipe[i].setScale(Vector2f(0, 0));
								sprite.upperpipe[i].setPosition(Vector2f(j + 720.0, 0.0));

								sprite.lowerpipe[i].setScale(Vector2f(0, 0));
								sprite.lowerpipe[i].setPosition(Vector2f(j + 720.0, yPos[i]));

								sprite.coin[i].setTexture(texture.coin);
								sprite.coin[i].setScale(Vector2f(0, 0));
								sprite.coin[i].setPosition(Vector2f(j + 720.0, coinPos[i]));

								j += 300.0; // distance between each pipe and coin

							}
							j = 0;
						}
						else if (mousePosition2.x > 250 && mousePosition2.x < 450 && mousePosition2.y > 480 && mousePosition2.y < 545)  //Ok

						{
							pause = false;
						}
					}

				}
			}
		}

		Fx = FlappyBird.sprite.getPosition().x; //Bird x-position
		Fy = FlappyBird.sprite.getPosition().y; //Bird y-position
		Fw = 16 * FlappyBird.sprite.getScale().x; //Bird x-scale
		Fh = 13 * FlappyBird.sprite.getScale().y; //Bird y-scale

		for (int i = 0; i < 7; i++)
		{
			Lx = (*(sprite.lowerpipe + i)).getPosition().x; //Lower pipe x-position
			Ly = (*(sprite.lowerpipe + i)).getPosition().y; //Lower pipe y-position
			Lw = 25 * (*(sprite.lowerpipe + i)).getScale().x; //Lower pipe x-scale
			Lh = 100 * (*(sprite.lowerpipe + i)).getScale().y; //Lower pipe y-scale

			Ux = (*(sprite.upperpipe + i)).getPosition().x; //Upper pipe x-position
			Uy = (*(sprite.upperpipe + i)).getPosition().y; //Upper pipe y-position
			Uw = 25 * (*(sprite.upperpipe + i)).getScale().x; //Upper pipe x-scale
			Uh = 150 * (*(sprite.upperpipe + i)).getScale().y; //Upper pipe y-scale

			if (collision(Fx, Fy, Fw, Fh, Lx, Ly, Lw, Lh, Ux, Uy, Uw, Uh)) //collision Function Call
			{
				collides = true;
			}
		}
		if (pause == false)
		{

			if (accClock.getElapsedTime().asSeconds() > 10)
			{
				jumpspeed += 0.5;
				accPipes -= 1.0;
				accClock.restart();
			}
			scoreTxt.setPosition(550, 0);

			for (int i = 0; i < 7; i++)
			{
				scoreTxt.setString("Score: " + to_string(score));

				if (FlappyBird.sprite.getGlobalBounds().intersects(sprite.coin[i].getGlobalBounds()))
				{
					Sounds.scoring.play();
					score++;
					sprite.coin[i].setScale(Vector2f(0, 0));
				}
			}
			sprite.pausemenu.setScale(0, 0);

			//Collision

			if (FlappyBird.sprite.getPosition().y > 795 || collides == true)
			{
				Sounds.collision.play();
				sprite.gameover.setColor(Color::White);
				sprite.scoreboard.setScale(4, 4);

				scoreTxt.setString(to_string(score));
				scoreTxt.setFillColor(Color::White);
				scoreTxt.setPosition(330, 420);

				highscore(score, hScore); //highscore Function Call

				highscoreTxt.setString(to_string(hScore));
				highscoreTxt.setFillColor(Color::White);
				highscoreTxt.setPosition(330, 510);

				sprite.getready.setColor(Color::Transparent);
				pause = true;
			}

			else

			{
				if (startClock.getElapsedTime().asSeconds() > 3)
				{
					sprite.pause.setScale(3, 3);
					sprite.getready.setColor(Color::Transparent);
					scoreTxt.setFillColor(Color::White);

					//Bird Motion

					if (Keyboard::isKeyPressed(Keyboard::Space))
					{
						Birdmotionup(jumpspeed, velocity); //Birdmotionup Function Call
						FlappyBird.sprite.setTextureRect(IntRect(0, 2 * 24, 20, 24));
						Sounds.flap.play();
					}
					else
					{
						Birdmotiondown(jumpspeed, velocity); //Birdmotiondown Function Call
						FlappyBird.sprite.setTextureRect(IntRect(0, 0, 20, 24));
					}
					FlappyBird.sprite.move(velocity);

					if (FlappyBird.sprite.getPosition().y < 0.f)
					{
						FlappyBird.sprite.setPosition(FlappyBird.sprite.getPosition().x, 0.f);
					}

					//Pipes & coins motion

					for (int i = 0; i < 7; i++)
					{
						velocityPipe.x = mpipe;

						if (sprite.upperpipe[i].getPosition().x + sprite.upperpipe[i].getScale().x < 6000)
						{
							Pipesmotion(velocityPipe, mpipe, accPipes); //Pipes motion Function Call
						}
						sprite.upperpipe[i].move(-velocityPipe.x, velocityPipe.y);
						sprite.lowerpipe[i].move(-velocityPipe.x, velocityPipe.y);
						sprite.coin[i].move(-velocityPipe.x, velocityPipe.y);

						if (sprite.lowerpipe[i].getPosition().x < -1000 && sprite.upperpipe[i].getPosition().x < -1000)
						{
							sprite.upperpipe[i].setPosition(1100, 0);
							sprite.lowerpipe[i].setPosition(1100, yPos[i]);
							sprite.coin[i].setPosition(1100, coinPos[i]);
							sprite.coin[i].setScale(Vector2f(0.22, 0.22));
						}
					}
				}
			}
		}
		window.clear();
		Draw(); //Draw Function Call
		window.display();
	}
}