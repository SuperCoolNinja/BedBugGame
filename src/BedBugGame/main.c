#include <stdlib.h>
#include <assert.h>
#include "../build/root.h"

/*@SuperCoolNinja*/

//MAX_TEXTURE :
#define MAX_BUG 5

//PROPERTY SCREEN SIZE :
#define WIDTH 800
#define HEIGHT 450


void StartTimer(Timer* timer, double lifetime)
{
	timer->StartTime = GetTime();
	timer->Lifetime = lifetime;
}

bool TimerDone(Timer timer)
{
	return GetTime() - timer.StartTime >= timer.Lifetime;
}

//Generate 5 bed bug texture :
static void generateDynamicMemoryForTextures(Texture2D* texture[])
{
	for (size_t i = 0; i < MAX_BUG; i++) {
		texture[i] = calloc(MAX_BUG, sizeof(Texture2D));
		if (texture[i] == NULL) perror("\nMemory cannot be allocated for texture arr[%d]\n", i);
		else printf("\nMemory allocated at : %p\n", texture[i]);
	}
}

//Build to each bedbug some property :
static void createDefaultBedBug(GameConfig config, Texture2D* texture[], Rectangle sourceRec[], Target bugTarget[], Texture2D newTexture)
{
	for (size_t i = 0; i < MAX_BUG; i++)
	{
		//Set the default texture :
		*texture[i] = newTexture;

		//Set the new data for the canvas :
		float frameHeight = (float)texture[i]->height;
		sourceRec[i].width = (float)texture[i]->width;
		sourceRec[i].height = frameHeight;

		//Set a random position for each target :
		bugTarget[i].property.x = rand() % config.width / 1.5f + 100;
		bugTarget[i].property.y = rand() % config.height / 1.5f + 100;

		//Set a width and height to each target :
		bugTarget[i].property.width = (float)texture[i]->width;
		bugTarget[i].property.height = frameHeight;
	}
}

//each frame we loop through and check if is the target is and the timer is endup then we apply the new data to it :
static void runtimeSetNewData(Target bugTarget[], Texture2D* texture[], Timer timer, GameConfig config, Texture2D newTexture)
{
	for (size_t i = 0; i < MAX_BUG; i++)
	{
		if (bugTarget[i].isTouch == true && TimerDone(timer))
		{
			bugTarget[i].property.x = rand() % config.width / 2.f + 120;
			bugTarget[i].property.y = rand() % config.height / 2.f + 120;
			*texture[i] = newTexture;
			bugTarget[i].isTouch = false;
			timer.StartTime = 1;
			break;
		}
	}
}


int main()
{
	//INIT RANDOM VALUE TO 0 : 
	srand(time(NULL));

	//Setup :
	GameConfig config = { WIDTH, HEIGHT, 0.f, 0.f };

	// set up the window property :
	InitWindow(config.width, config.height, "BedBugGame");

	//Some struct :
	Texture2D textureAlive = LoadTexture("resources/boss.png");
	Texture2D textureDead = LoadTexture("resources/boss_dead.png");
	Timer timer = { 1, 0.0 };
	Texture2D* texture[MAX_BUG] = { NULL };
	Rectangle sourceRec[MAX_BUG] = { 0.f, 0.f, 0.f, 0.f };
	Target bugTarget[MAX_BUG];

	//Make a dynamic allocation for each data structure that will contain our texture in heap :
	generateDynamicMemoryForTextures(texture);

	//Check if everything did allocate correctly :
	assert(texture != NULL && "Texture memory did not allocate correctly.");
	assert(*texture != NULL && "Texture data was NULL.");

	//Setup some default value to our texture like pos and stuff :
	createDefaultBedBug(config, texture, sourceRec, bugTarget, textureAlive);

	// Set our game to run at 60 frames-per-second :
	SetTargetFPS(60);

	// game loop
	while (!WindowShouldClose())
	{
		//Init the new position of the mouse with our data :
		config.mousePoint.x = GetMousePosition().x;
		config.mousePoint.y = GetMousePosition().y;

		// drawing
		BeginDrawing();
		ClearBackground(RAYWHITE);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			for (size_t i = 0; i < MAX_BUG; i++)
			{
				if (CheckCollisionPointRec(config.mousePoint, bugTarget[i].property))
				{
					StartTimer(&timer, 0.1);
					*texture[i] = textureDead;
					bugTarget[i].isTouch = true;
					break;
				}
			}
		}

		runtimeSetNewData(bugTarget, texture, timer, config, textureAlive);

		for (size_t i = 0; i < MAX_BUG; i++)
			DrawTextureRec(*texture[i], *sourceRec, (Vector2) { bugTarget[i].property.x, bugTarget[i].property.y }, WHITE);

		EndDrawing();
	}

	for (size_t i = 0; i < MAX_BUG; i++) {
		printf("\nBEFORE : Cleanup Memory allocated at : %p\n", texture[i]);
		UnloadTexture(*texture[i]);
		free(texture[i]);
		texture[i] = NULL;
		printf("AFTER : Cleanup Memory allocated at : %p\n", texture[i]);
	}

	// cleanup
	CloseWindow();
	return 0;
}