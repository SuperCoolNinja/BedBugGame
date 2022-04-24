#pragma once

typedef struct
{
	int width;
	int height;
	Vector2 mousePoint;
} GameConfig;

typedef struct
{
	Rectangle property;
	bool isTouch;
} Target;

typedef struct
{
	double StartTime;
	double Lifetime;
}Timer;