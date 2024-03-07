#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define windowWidth 480
#define windowHeight 512
#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

// All flags related to gamepages and game-level is defined here
enum gameStageflag
{
	on,
	off,
	home,
	gamePage,
	setting,
	score,
	gameover,
	preGame,
	easy,
	medium,
	hard,
	newgame,
	resume,
	infoPage
};

// All necessary variable required to keep track of the gamestages and gamelevel is defined here
struct Game
{
	int gameState = newgame;
	int window = home;
	int sound = off;
	int music = off;
	int easyGame = -1;
	int mediumGame = -1;
	int hardGame = -1;
	int gamelevel = hard;
	int startOver = on;
	int score = 0;
	int lives = 3;
	bool modifiedColor = false;
	bool landingEvent = false;
	char *nametext[11] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	char *scoretext[11] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	int levels[11] = {0};
	char *str;
	char *playerName = NULL;
	int objectSelected = 0;
	bool colorError = false;
	char colorText[20];
	int playerNameLength = 0;
	int leaderCount = 0;
	int nearestPlatform;
	int bgx = 0;
	int bgy = 0;
	int bgi = 0;
} game;

// Strcuture to hold information related to ball
struct Ball
{
	double x = 256;
	double y = 256;
	double rad = 6;
	double speed;
};
Ball ball;

// Structure to hold necessary info about platform
struct platform
{
	double x;
	double y;
	double length = 65;
	double width = 15;
	int speedX = 0;
};

// Color structure
typedef struct
{
	int r;
	int g;
	int b;
} Color;

Color colors[3] = {{150, 20, 20}, {0, 252, 50}, {150, 150, 180}};
Color customColor, ballcolor = {50, 78, 19};

// structure to hold random Objects
struct Objects
{
	int x;
	int y;
	char *gui = NULL;
	int speedY;
	int height;
	int width;
	bool exist = false;
};

Objects object[4];
platform arr[6];
char bg[51][40] = {"images//bg//sky11.bmp", "images//bg//sky12.bmp", "images//bg//sky13.bmp", "images//bg//sky14.bmp", "images//bg//sky15.bmp",
				   "images//bg//sky16.bmp", "images//bg//sky17.bmp", "images//bg//sky18.bmp", "images//bg//sky19.bmp", "images//bg//sky110.bmp",
				   "images//bg//sky111.bmp", "images//bg//sky112.bmp", "images//bg//sky113.bmp", "images//bg//sky115.bmp", "images//bg//sky116.bmp",
				   "images//bg//sky117.bmp", "images//bg//sky118.bmp", "images//bg//sky119.bmp", "images//bg//sky120.bmp", "images//bg//sky121.bmp",
				   "images//bg//sky122.bmp", "images//bg//sky123.bmp", "images//bg//sky124.bmp", "images//bg//sky125.bmp", "images//bg//sky126.bmp",
				   "images//bg//sky127.bmp", "images//bg//sky128.bmp", "images//bg//skye1.bmp", "images//bg//sky1e2.bmp", "images//bg//sky1e3.bmp",
				   "images//bg//sky1e4.bmp", "images//bg//sky1e5.bmp", "images//bg//sky1e6.bmp", "images//bg//sky1e7.bmp", "images//bg//sky1e8.bmp",
				   "images//bg//sky1e9.bmp", "images//bg//sky1e10.bmp", "images//bg//sky1e11.bmp", "images//bg//sky1e12.bmp", "images//bg//sky1e13.bmp",
				   "images//bg//sky1e14.bmp", "images//bg//sky1e15.bmp", "images//bg//sky1e17.bmp", "images//bg//sky1e18.bmp", "images//bg//sky1e19.bmp", "images//bg//sky1e20.bmp",
				   "images//bg//sky1e21.bmp", "images//bg//sky1e22.bmp", "images//bg//sky1e23.bmp", "images//bg//sky1e24.bmp", "images//bg//sky1e25.bmp"};

int shape[15][65];
void loadShape()
{
	FILE *filePointer = fopen("shape.txt", "r");

	int Row = 15;
	int Col = 65;
	for (int row = 0; row < Row; row++)
	{
		for (int col = 0; col < Col; col++)
		{
			fscanf(filePointer, "%d", &shape[row][col]);
		}
	}
	fclose(filePointer);
}

void drawShape(double X, double Y, Color color)
{
	for (int row = 0; row < 15; row++)
	{
		for (int col = 0; col < 65; col++)
		{
			if (shape[row][col] == 0)
			{
				iSetColor(color.r, color.g, color.b);
				iFilledRectangle(X + col, Y + row, 1, 1);
			}
			else if (shape[row][col] == 2)
			{
				iSetColor(0, 0, 0);
				iFilledRectangle(X + col, Y + row, 1, 1);
			}
		}
	}
}

void objectMovement()
{
	for (int i = 0; i < 4; i++)
	{
		if (i == 1)
		{
			if (object[1].exist)
				printf("%d %d", object[1].x, object[1].y);
		}
		object[i].y += object[i].speedY;
		if (object[i].speedY > 0 && object[i].y > windowHeight)
		{
			object[i].speedY = 0;
			object[i].exist = false;
		}
		if (object[i].speedY < 0 && object[i].y < 0)
		{
			object[i].speedY = 0;
			object[i].exist = false;
		}
	}
}

void randomEvent()
{
	int eventNo = rand() % 500;
	switch (eventNo)
	{
	case 1:
	{
		if (object[0].exist)
			break;
		if (object[0].gui == NULL)
		{
			char *str = "images/lives1.bmp";
			int size = strlen(str) + 1;
			object[0].gui = (char *)malloc(size * sizeof(char));
			strcpy(object[0].gui, str);
		}
		object[0].height = 16;
		object[0].width = 16;
		object[0].x = rand() % (windowWidth - 32);
		if (rand() % 2)
		{
			object[0].y = windowHeight + 10;
			object[0].speedY = -(rand() % (6 - 1) + 1);
		}
		else
		{
			object[0].y = -32;
			object[0].speedY = rand() % (6 - 1) + 1;
		}
		object[0].exist = true;
		break;
	}
	case 2:
	{
		if (arr[game.nearestPlatform].y < windowHeight / 2)
			break;
		arr[game.nearestPlatform].y = 2 * windowHeight;
		break;
	}
	}
}

// Function to control Movement of the platform which is called again and again
void movingCharacteristic(int index)
{
	// to create variation in the movement characteristic randomization algorithm is heavily used
	// one to create variation in the number of moving platfomr
	// one to decide which should be moving
	// one to decide speed
	if (arr[index].speedX != 0)
	{
		// to prevent this new platform from moving based on decisio of other scenario.
		arr[index].speedX = 0;
	}
	int movingPlatformCount = 0;
	for (int i = 0; i < 6; i++)
	{
		if (arr[i].speedX != 0)
			movingPlatformCount++;
	}
	int randFactor = rand() % 3;
	if (randFactor > movingPlatformCount && (rand() % 2))
	{
		arr[index].speedX = rand() % 5 + 1;
		// to create variation in the movement direction starting
		if (rand() % 2)
			arr[index].speedX = -arr[index].speedX;
	}
}

void getNewPlatform(int index)
{
	int minimumY = windowHeight, maximumDistance;
	for (int i = 0; i < 6; i++)
	{
		minimumY = min((int)arr[i].y, minimumY);
	}
	minimumY -= 50;
	maximumDistance = minimumY - 100;
	// to make the platform sufficiently spaced in the y axis
	arr[index].y = rand() % (minimumY - maximumDistance) + maximumDistance;
	arr[index].x = rand() % (windowWidth - (int)arr[index].length);
	// the ideal Spwaning region should be below the frame.To maintain this property and add the effect of variablity
	// follow procedures is taken
	int idealSpawningY = rand() % (2 * (int)arr[index].width) - 6 * (int)arr[index].width;
	arr[index].y = min(arr[index].y, idealSpawningY);
	movingCharacteristic(index);
}

void platformMovement()
{

	if (game.startOver == on)
	{
		arr[0].y = -3 * arr[0].width;
		arr[0].x = rand() % (int)(windowWidth - arr[0].length / 4);
		for (int i = 1; i < 6; i++)
		{
			arr[i].y = rand() % (int)(arr[i - 1].y + windowHeight / 4 - arr[i - 1].y - 3 * arr[i].width) + arr[i - 1].y + 3 * arr[i].width;
			arr[i].x = rand() % (int)(windowWidth - arr[0].length);
		}
		ball.x = max(6, min(arr[2].x + arr[0].length / 2, windowWidth - 2 * ball.rad));
		ball.y = arr[2].y + arr[2].width + ball.rad;
		game.lives = 3;
		game.startOver = off;
	}
	else
	{

		for (int i = 0; i < 6; i++)
		{
			if (arr[i].y >= windowHeight)
			{
				getNewPlatform(i);
			}
			else
			{
				arr[i].y += 2;
				if (arr[i].speedX != 0)
				{
					if (arr[i].speedX > 0)
					{
						if (arr[i].x + arr[i].length + arr[i].speedX < windowWidth)
						{
							arr[i].x += arr[i].speedX;
						}
						else
							arr[i].speedX = -arr[i].speedX;
					}
					else
					{
						if (arr[i].x + arr[i].speedX > 0)
						{
							arr[i].x += arr[i].speedX;
						}
						else
							arr[i].speedX = -arr[i].speedX;
					}
				}
			}
		}
	}
	randomEvent();
	objectMovement();
}

void resumeGame();

void pauseGame();
/*Whenever the ball goes beyond valid region but player still has remaining lives this
function is called from the ballMovement fuction to spawn the ball at proper location*/
void reSpawn()
{

	for (int i = 0; i < 6; i++)
	{
		if (arr[i].y > 0 && arr[i].y <= 3 * windowHeight / 4)
		{
			ball.y = arr[i].y + arr[i].width + ball.rad;
			ball.x = arr[i].x + arr[i].length / 2;
			resumeGame();
			return;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		if (arr[i].y < windowHeight)
		{
			ball.y = arr[i].y + arr[i].width + ball.rad;
			ball.x = arr[i].x + arr[i].length / 2;
			resumeGame();
			return;
		}
	}
	ball.y = windowHeight - 2 * ball.rad;
	ball.x = windowWidth / 2;
	resumeGame();
}
// Function to control movement of the ball.As the movement direction depends on collision
// Collision Detection is also done in the same function
void ballMovement()
{
	for (int i = 0; i < 4; i++)
	{
		int Y = (int)ball.y;
		int X = (int)ball.x;
		int R = (int)ball.rad;
		if (Y >= object[i].y - R && Y <= object[i].y + object[i].height + R && X >= object[i].x - R && X <= object[i].x + object[i].width + R && object[i].exist)
		{
			if (game.lives < 3)
				game.lives++;
			object[i].exist = false;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		int dis = ball.y - arr[i].y - 21;
		if (dis <= 3 && dis >= -3 && ball.x >= arr[i].x && ball.x <= arr[i].x + arr[i].length)
		{
			ball.y += 3;
			if (game.landingEvent)
			{
				game.score += 25;
				game.landingEvent = false;
			}
			ball.x += arr[i].speedX + (double)arr[i].speedX / 7; // to counteract the tiny movement it shows.
			game.nearestPlatform = i;
			return;
		}
	}
	ball.y -= 5;
	if (ball.y < 0 || ball.y > windowHeight)
	{
		if (game.sound == on)
		{
			PlaySound(TEXT("music//died.wav"), NULL, SND_FILENAME | SND_SYNC);
			if (game.music == on)
			{
				PlaySound(TEXT("music/background3.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
		}
		else
			Sleep(500);
		pauseGame();
		if (game.lives <= 1)
		{
			game.window = gameover;
		}
		else
		{
			game.lives--;
			reSpawn();
		}
	}
	else
		game.landingEvent = true;
}

void clearLeaderBoard()
{
	FILE *filePointer = fopen("scores.txt", "w");
	fclose(filePointer);

	for (int i = 0; i < 11; i++)
	{
		game.nametext[i] = NULL;
		game.scoretext[i] = NULL;
		game.levels[i] = 0;
	}
}

int fillLeaderboard()
{
	FILE *filePointer = fopen("scores.txt", "r");
	if (filePointer == NULL)
	{
		filePointer = fopen("scores.txt", "w");
	}
	char c;
	game.str = (char *)malloc(1000 * sizeof(char));
	memset(game.str, '\0', 1000);
	int index = 0;
	while ((fscanf(filePointer, "%c", &c)) != EOF)
	{
		game.str[index] = c;
		index++;
	}
	fclose(filePointer);
	char *string = strtok(game.str, "\n");
	char *array[10];
	int n;
	for (n = 0; string != NULL; n++)
	{
		array[n] = string;
		// printf("%s\n",string);
		string = strtok(NULL, "\n");
	}
	for (int i = 0; i < n; i++)
	{
		game.nametext[i] = strtok(array[i], ",");
		game.scoretext[i] = strtok(NULL, ",");
		game.levels[i] = atoi(strtok(NULL, ","));
		// printf("(%s) (%s) (%d)\n",nametext[i],scoretext[i],levels[i]);
	}
	return n;
}

void cleanUp()
{
	for (int i = 0; i < 11; i++)
	{
		game.nametext[i] = NULL;
		game.scoretext[i] = NULL;
		game.levels[i] = 0;
	}
	for (int i = 0; i < 4; i++)
	{
		if (object[i].gui != NULL)
			free(object[i].gui);
	}
	if (game.str != NULL)
	{
		free(game.str);
		game.str = NULL;
	}
}

void updateLeaderboard(char *name, int score, int level)
{
	int n = fillLeaderboard();
	int scoreArray[n + 1] = {0};
	for (int i = 0; i < n; i++)
	{
		scoreArray[i] = atoi(game.scoretext[i]);
	}
	int index = 0;
	for (int i = 0; i <= n; i++)
	{
		if (score > scoreArray[i])
		{
			index = i;
			break;
		}
	}
	for (int i = n; i > index; i--)
	{
		game.scoretext[i] = game.scoretext[i - 1];
		game.nametext[i] = game.nametext[i - 1];
		game.levels[i] = game.levels[i - 1];
	}
	game.nametext[index] = (char *)malloc(20 * sizeof(char));
	game.scoretext[index] = (char *)malloc(10 * sizeof(char));
	game.levels[index] = level;
	sprintf(game.nametext[index], "%s", name);
	sprintf(game.scoretext[index], "%d", score);
	FILE *filePointer = fopen("scores.txt", "w");
	for (int i = 0; i < min(10, n + 1); i++)
	{
		// printf("234(%s) (%s) (%d)\n",nametext[i],scoretext[i],levels[i]);
		fprintf(filePointer, "%s,%s,%d\n", game.nametext[i], game.scoretext[i], game.levels[i]);
	}
	fclose(filePointer);
	cleanUp();
}

void resumeGame()
{
	if (game.gamelevel == easy)
	{
		iResumeTimer(game.easyGame);
	}
	else if (game.gamelevel == medium)
	{
		iResumeTimer(game.mediumGame);
	}
	else
		iResumeTimer(game.hardGame);
}

void pauseGame()
{
	if (game.gamelevel == easy)
	{
		iPauseTimer(game.easyGame);
	}
	else if (game.gamelevel == medium)
	{
		iPauseTimer(game.mediumGame);
	}
	else
		iPauseTimer(game.hardGame);
}

void gameMovement()
{
	platformMovement();
	ballMovement();
}

void setTimer()
{
	if (game.gamelevel == easy && game.easyGame == -1)
	{
		game.easyGame = iSetTimer(45, gameMovement);
	}
	else if (game.gamelevel == medium && game.mediumGame == -1)
	{
		game.mediumGame = iSetTimer(40, gameMovement);
	}
	else if (game.gamelevel == hard && game.hardGame == -1)
	{
		game.hardGame = iSetTimer(30, gameMovement);
	}
	pauseGame();
}

void newGame()
{
	game.startOver = on;
	game.score = 0;
	platformMovement();
}
// The main rendering function which is called again and again
void setBackground()
{
	static int counter = 0;
	if (counter >= 50)
	{
		game.bgi++;
		counter = 0;
	}
	else
		counter++;
	if (game.bgi >= 51)
		game.bgi = 0;
	iShowBMP(game.bgx, game.bgy, bg[game.bgi]);
}

void setHome()
{
	iShowBMP(145, 390, "images//game2.bmp");
	iSetColor(0, 0, 0);

	iShowBMP(320, 336, "images//blue_button01.bmp");
	iText(332, 341, "SETTINGS", GLUT_BITMAP_HELVETICA_18);
	iShowBMP(40, 336, "images//blue_button01.bmp");
	iText(59, 341, "SCORES", GLUT_BITMAP_HELVETICA_18);
	iShowBMP(320, 96, "images//blue_button01.bmp");
	iText(360, 101, "EXIT", GLUT_BITMAP_HELVETICA_18);
	iShowBMP(40, 96, "images//blue_button01.bmp");
	iText(45, 101, "CUSTOMIZE", GLUT_BITMAP_HELVETICA_18);
	if (game.gameState == resume)
	{
		iShowBMP(180, 250, "images//blue_button01.bmp");
		iText(200, 255, "RESUME", GLUT_BITMAP_HELVETICA_18);
		iShowBMP(180, 182, "images//blue_button01.bmp");
		iText(190, 187, "NEW GAME", GLUT_BITMAP_HELVETICA_18);
	}
	else
	{
		iShowBMP(180, 210, "images//blue_button01.bmp");
		iText(190, 215, "NEW GAME", GLUT_BITMAP_HELVETICA_18);
	}
}

void drawBar(int x, int y)
{
	iSetColor(0, 0, 0);
	iFilledRectangle(x, y - 1, 40, 1);
	iFilledRectangle(x + 50, y - 1, 40, 1);
	iFilledRectangle(x + 100, y - 1, 40, 1);
	for (int i = 0; i < 255; i++)
	{
		iSetColor(1 + i, 0, 0);
		iFilledRectangle(x, y + i, 40, 1);
		iSetColor(0, i + 1, 0);
		iFilledRectangle(x + 50, y + i, 40, 1);
		iSetColor(0, 0, i + 1);
		iFilledRectangle(x + 100, y + i, 40, 1);
	}
	iSetColor(255, 0, 0);
	iFilledRectangle(x, y + 255, 40, 1);
	iSetColor(0, 255, 0);
	iFilledRectangle(x + 50, y + 255, 40, 1);
	iSetColor(0, 0, 255);
	iFilledRectangle(x + 100, y + 255, 40, 1);
}

void showColor(Color color, int x, int y)
{
	iSetColor(255, 255, 255);
	iFilledRectangle(x, y + color.r, 40, 3);
	iFilledRectangle(x + 50, y + color.g, 40, 3);
	iFilledRectangle(x + 100, y + color.b, 40, 3);
}

void iDraw()
{
	iClear();
	iSetColor(256, 256, 256);
	iFilledRectangle(0, 0, windowWidth, windowHeight);
	switch (game.window)
	{
	case home:
	{

		setHome();
		break;
	}
	case preGame:
	{
		iSetColor(0, 0, 0);
		iShowBMP2(180, 300, "images//green_button01.bmp", 0);
		iText(215, 306, "EASY", GLUT_BITMAP_HELVETICA_18);
		iShowBMP2(180, 250, "images//yellow_button01.bmp", 0);
		iText(203, 256, "MEDIUM", GLUT_BITMAP_HELVETICA_18);
		iShowBMP2(180, 200, "images//red_button01.bmp", 0);
		iText(215, 206, "HARD", GLUT_BITMAP_HELVETICA_18);
		iShowBMP2(180, 150, "images//blue_button01.bmp", 0);
		iText(200, 156, "GO BACK", GLUT_BITMAP_HELVETICA_18);
		break;
	}
	case gamePage:
	{
		setBackground();
		iSetColor(0, 0, 0);
		char scoreText[15];
		sprintf(scoreText, "Score: %d", game.score);
		iText(20, windowHeight - 40, scoreText, GLUT_BITMAP_HELVETICA_12);
		iShowBMP2(windowWidth - 32, windowHeight - 32, "images/homebutton1.bmp", 0);
		for (int i = 0; i < 4; i++)
		{
			if (object[i].exist)
			{
				iShowBMP2(object[i].x, object[i].y, object[i].gui, 0);
			}
		}
		for (int i = 1; i <= game.lives; i++)
		{
			iShowBMP2(20 * i, windowHeight - 24, "images/lives1.bmp", 0);
		}
		iSetColor(ballcolor.r, ballcolor.g, ballcolor.b);
		iFilledCircle(ball.x, ball.y, ball.rad);
		for (int i = 0; i < 6; i++)
		{
			if (game.gamelevel == hard)
			{
				drawShape(arr[i].x, arr[i].y, colors[0]);
			}
			else if (game.gamelevel == easy)
			{
				drawShape(arr[i].x, arr[i].y, colors[1]);
			}
			else if (game.gamelevel == medium)
			{
				drawShape(arr[i].x, arr[i].y, colors[2]);
			}
		}
		break;
	}
	case score:
	{
		iSetColor(0, 0, 0);
		iText(160, 480, "LEADERBOARD", GLUT_BITMAP_TIMES_ROMAN_24);
		if (game.nametext[0] == NULL)
		{
			game.leaderCount = fillLeaderboard();
		}
		iShowBMP(10, 5, "images//blue_button03.bmp");
		iText(14, 10, "GO BACK", GLUT_BITMAP_HELVETICA_18);
		iShowBMP(370, 5, "images//red_button03.bmp");
		iText(390, 10, "CLEAR", GLUT_BITMAP_HELVETICA_18);
		iShowBMP(10, 440, "images//blue_button02.bmp");
		iText(15, 450, "RANK", GLUT_BITMAP_HELVETICA_18);
		iText(90, 450, "NAME", GLUT_BITMAP_HELVETICA_18);
		iText(230, 450, "SCORE", GLUT_BITMAP_HELVETICA_18);
		iText(400, 450, "LEVEL", GLUT_BITMAP_HELVETICA_18);
		int startY = 400;
		for (int i = 0; i < game.leaderCount; i++)
		{
			// Constants used in this part has been deduced by trial and error method
			char rankNumber[5];
			sprintf(rankNumber, "  %d.", i + 1);
			if (game.levels[i] == 8)
			{
				iShowBMP(10, startY - i * 40, "images//green_button02.bmp");
				iText(400, 410 - 40 * i, "EASY");
			}
			else if (game.levels[i] == 9)
			{
				iShowBMP(10, startY - i * 40, "images//yellow_button02.bmp");
				iText(400, 410 - 40 * i, "MEDIUM");
			}
			else
			{
				iShowBMP(10, startY - i * 40, "images//red_button02.bmp");
				iText(400, 410 - 40 * i, "HARD");
			}
			iText(15, 410 - 40 * i, rankNumber, GLUT_BITMAP_HELVETICA_18);
			iText(90, 410 - 40 * i, game.nametext[i], GLUT_BITMAP_HELVETICA_18);
			iText(230, 410 - 40 * i, game.scoretext[i], GLUT_BITMAP_HELVETICA_18);
		}
		break;
	}
	case gameover:
	{
		iShowBMP(90, 208, "images//Untitled-2.bmp");
		iSetColor(255, 0, 0);
		if (game.playerName != NULL)
			iText(130, 310, game.playerName, GLUT_BITMAP_HELVETICA_18);
		break;
	}
	case setting:
	{
		iSetColor(255, 0, 0);
		iShowBMP(windowWidth / 2 - 16, 316, "images/sound.bmp");
		if (game.sound == off)
		{
			iLine(windowWidth / 2 - 10, 320, windowWidth / 2 + 10, 344);
			iLine(windowWidth / 2 - 10, 344, windowWidth / 2 + 10, 320);
		}
		iShowBMP(windowWidth / 2 - 16, 268, "images/musicplay.bmp");
		if (game.music == off)
		{
			iLine(windowWidth / 2 - 10, 272, windowWidth / 2 + 10, 296);
			iLine(windowWidth / 2 - 10, 296, windowWidth / 2 + 10, 272);
		}
		iShowBMP2(windowWidth / 2 - 16, 220, "images/gobackbutton.bmp", 0);
		break;
	}
	case infoPage:
	{
		iShowBMP(50, 50, "images//blue_button01.bmp");
		iSetColor(0, 0, 0);
		iText(140, 450, "Welcome To Customization Page!!!");
		iText(20, 430, "You can customize the color of platform and ball as you");
		iText(20, 410, "wish.To change the color of platform and ball first select");
		iText(20, 390, "the object then give the rgb value of the color in the ");
		iText(20, 370, "black field.Don't worry if You don't know the rgb values.");
		iText(20, 350, "Just try and see the result immediately!!!");
		iText(20, 330, "Platform:");
		iText(20, 310, "EASY MODE:");
		iText(20, 280, "MEDIUM MODE:");
		iText(20, 250, "EASY MODE:");
		if (game.objectSelected == 1)
		{
			iRectangle(115, 305, 65, 16);
		}
		else if (game.objectSelected == 2)
		{
			iRectangle(115, 274, 65, 16);
		}
		else if (game.objectSelected == 3)
		{
			iRectangle(115, 244, 65, 16);
		}
		else if (game.objectSelected == 4)
		{
			iRectangle(73, 216, 14, 14);
		}
		drawShape(115, 306, colors[1]);
		drawShape(115, 275, colors[2]);
		drawShape(115, 245, colors[0]);
		iText(20, 220, "Ball:");
		iSetColor(ballcolor.r, ballcolor.g, ballcolor.b);
		iFilledCircle(80, 223, ball.rad);
		iSetColor(0, 0, 0);
		iText(65, 56, "GO BACK", GLUT_BITMAP_HELVETICA_18);
		drawBar(300, 90);
		if (game.objectSelected == 1)
		{
			showColor(colors[1], 300, 90);
		}
		else if (game.objectSelected == 2)
		{
			showColor(colors[2], 300, 90);
		}
		else if (game.objectSelected == 3)
		{
			showColor(colors[0], 300, 90);
		}
		else if (game.objectSelected)
		{
			showColor(ballcolor, 300, 90);
		}
		break;
	}
	}
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my)
{
	// printf("x = %d, y= %d\n", mx, my);
	//  place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my)
{
	switch (game.window)
	{
	case home:
	{

		if (mx >= 320 && mx <= 440 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (my >= 96 && my <= 121)
			{
				exit(0);
			}
			else if (my >= 336 && my <= 361)
			{
				game.window = setting;
			}
		}
		else if (mx >= 40 && mx <= 160 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (my >= 96 && my <= 121)
			{
				game.window = infoPage;
			}
			else if (my >= 336 && my <= 361)
			{
				game.window = score;
			}
		}
		else if (mx >= 180 && mx <= 300 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (game.gameState == resume)
			{
				if (my >= 250 && my <= 275)
				{
					resumeGame();
					game.window = gamePage;
				}
				else if (my >= 182 && my <= 207)
				{
					newGame();
					game.window = preGame;
				}
			}
			else
			{
				if (my >= 210 && my <= 235)
				{
					game.window = preGame;
				}
			}
		}
		break;
	}
	case preGame:
	{
		if (mx >= 180 && mx <= 300 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (my >= 300 && my <= 325)
			{
				game.gamelevel = easy;
				setTimer();
				resumeGame();
				game.window = gamePage;
			}
			else if (my >= 250 && my <= 275)
			{
				game.gamelevel = medium;
				setTimer();
				resumeGame();
				game.window = gamePage;
			}
			else if (my >= 200 && my <= 225)
			{
				game.gamelevel = hard;
				setTimer();
				resumeGame();
				game.window = gamePage;
			}
			else if (my >= 150 && my <= 175)
			{
				game.window = home;
			}
		}
		break;
	}
	case gamePage:
		if (mx >= windowWidth - 32 && mx <= windowWidth && my >= windowHeight - 32 && my <= windowHeight && button == GLUT_LEFT_BUTTON)
		{
			game.window = home;
			game.gameState = resume;
			pauseGame();
		}
		break;
	case score:
	{
		if (mx >= 10 && mx <= 110 && my >= 5 && my <= 30 && button == GLUT_LEFT_BUTTON)
		{
			game.window = home;
			cleanUp();
		}
		else if (mx >= 370 && mx <= 470 && my >= 5 && my <= 30 && button == GLUT_LEFT_BUTTON)
		{
			clearLeaderBoard();
		}
		break;
	}
	case gameover:
	{
		if (mx >= 147 && mx <= 222 && my >= 225 && my <= 268 && button == GLUT_LEFT_BUTTON)
		{
			if (game.playerName != NULL)
			{
				free(game.playerName);
				game.playerName = NULL;
				game.playerNameLength = 0;
			}
			newGame();
			game.window = home;
			game.gameState = newgame;
		}
		else if (mx >= 252 && mx <= 327 && my >= 225 && my <= 268 && button == GLUT_LEFT_BUTTON)
		{
			if (game.playerName != NULL)
			{
				free(game.playerName);
				game.playerName = NULL;
				game.playerNameLength = 0;
			}
			newGame();
			game.window = gamePage;
			resumeGame();
		}
		else if (mx >= 125 && mx <= 298 && my >= 304 && my <= 328 && button == GLUT_LEFT_BUTTON && game.playerNameLength == 0)
		{
			if (game.score)
			{
				game.playerName = (char *)malloc(20 * sizeof(char));
				memset(game.playerName, '\0', 20);
				game.playerName[0] = '|';
				game.playerNameLength = 1;
			}
		}
		break;
	}
	case setting:
	{
		if (mx >= windowWidth / 2 - 16 && mx <= windowWidth / 2 + 16 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (my >= 220 && my <= 252)
			{
				game.window = home;
			}
			else if (my >= 268 && my <= 300)
			{
				if (game.music == on)
				{
					PlaySound(NULL, NULL, SND_ASYNC);
					game.music = off;
				}
				else
				{
					PlaySound(TEXT("music/background3.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
					game.music = on;
				}
			}
			else if (my >= 316 && my <= 348)
			{

				if (game.sound == on)
				{
					game.sound = off;
				}
				else
				{
					game.sound = on;
				}
			}
		}
		break;
	}
	case infoPage:
	{
		if (mx >= 50 && mx <= 170 && my >= 50 && my <= 75 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			game.window = home;
			game.objectSelected = 0;
			game.colorError = false;
		}
		else if (mx >= 115 && mx <= 180 && my >= 306 && my <= 321 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			game.objectSelected = 1;
		}
		else if (mx >= 115 && mx <= 180 && my >= 275 && my <= 290 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			game.objectSelected = 2;
		}
		else if (mx >= 115 && mx <= 180 && my >= 245 && my <= 260 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			game.objectSelected = 3;
		}
		else if (mx >= 74 && mx <= 86 && my >= 217 && my <= 229 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			game.objectSelected = 4;
		}
		else if (game.objectSelected != 0)
		{
			if (mx >= 300 && mx <= 340 && my >= 90 && my <= 342 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
				switch (game.objectSelected)
				{
				case 1:
					colors[1].r = my - 90;
					break;
				case 2:
					colors[2].r = my - 90;
					break;
				case 3:
					colors[0].r = my - 90;
					break;
				case 4:
					ballcolor.r = my - 90;
					break;
				}
			}
			else if (mx >= 350 && mx <= 390 && my >= 90 && my <= 342 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
				switch (game.objectSelected)
				{
				case 1:
					colors[1].g = my - 90;
					break;
				case 2:
					colors[2].g = my - 90;
					break;
				case 3:
					colors[0].g = my - 90;
					break;
				case 4:
					ballcolor.g = my - 90;
					break;
				}
			}
			else if (mx >= 400 && mx <= 440 && my >= 90 && my <= 342 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
				switch (game.objectSelected)
				{
				case 1:
					colors[1].b = my - 90;
					break;
				case 2:
					colors[2].b = my - 90;
					break;
				case 3:
					colors[0].b = my - 90;
					break;
				case 4:
					ballcolor.b = my - 90;
					break;
				}
			}
		}
		break;
	}
	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key)
{
	switch (game.window)
	{
	case gameover:
	{
		if (game.playerName != NULL)
		{
			if (key != 13 && key != 8 && key != 127 && game.playerNameLength < 15)
			{
				game.playerName[game.playerNameLength] = '|';
				game.playerName[game.playerNameLength - 1] = key;
				game.playerNameLength++;
			}
			else if (key == 8)
			{
				if (game.playerNameLength > 1)
				{
					game.playerName[game.playerNameLength - 1] = '\0';
					game.playerName[game.playerNameLength - 2] = '|';
					game.playerNameLength--;
				}
			}
			else if (key == 13)
			{
				game.playerName[game.playerNameLength - 1] = '\0';
				updateLeaderboard(game.playerName, game.score, game.gamelevel);
				game.playerName = NULL;
				game.playerNameLength = 0;
				game.score = 0;
			}
			else if (key == 127)
			{
				free(game.playerName);
				game.playerName = NULL;
				game.playerNameLength = 0;
			}
		}
		break;
	}
	case infoPage:
	{
		break;
	}
	}
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key)
{
	switch (game.window)
	{
	case gamePage:
	{
		if (key == GLUT_KEY_LEFT)
		{
			if (ball.x > ball.rad)
				ball.x -= 5;
		}
		if (key == GLUT_KEY_RIGHT)
		{
			if (ball.x < windowWidth - ball.rad)
				ball.x += 5;
		}
		break;
	}
	case gameover:
	{
		///
	}
	}
}

// The game starts here
int main()
{
	loadShape();
	srand(time(NULL));
	if (game.music == on)
		PlaySound(TEXT("music/background3.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	iInitialize(windowWidth, windowHeight, "Platform404");
	cleanUp();
	return 0;
}
