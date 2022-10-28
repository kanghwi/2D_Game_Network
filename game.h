#pragma once
#include <iostream>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include <thread>
#include <random>

#include "Extra_Libs\SDL2-2.24.0\include\SDL.h"
#include "Extra_Libs\SDL2_image-2.6.2\include\SDL_image.h"
#include "Extra_Libs\SDL2_mixer-2.6.2\include\SDL_mixer.h"
#include "Extra_Libs\SDL2_ttf-2.0.15\include\SDL_ttf.h"
using namespace std;

#define WIDTH 1200
#define HEIGHT 900

typedef struct TWO_Floats {
	float x;
	float y;
}TF;

typedef struct TWO_Ints {
	int x;
	int y;
}TI;

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* tmpSurface;
	SDL_Event event;

	SDL_Texture* playerTex;
	SDL_Texture* targetTex;
	SDL_Texture* backTex;
	SDL_Texture* bulletTex;
	SDL_Texture* flashTex;

	Mix_Chunk* gunsound;

	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;

	TTF_Font* font;
	SDL_Color white;

	TF MyCharPos;
	TF MyVelo;
	TF mouse_point;
	TF middle_pos;
	TF myBulletPos;
	TF myBulletVelo;

	TI flash_size;

	SDL_Rect srcR, destR;
	SDL_Point center;

	int iMonitorWidth;    // 모니터 가로 해상도
	int iMonitorHeight; //새로  해상도

	int window_moved_x = 0;
	int window_moved_y = 0;

	bool w_Pressed = false;
	bool a_Pressed = false;
	bool s_Pressed = false;
	bool d_Pressed = false;

	int mouse_X = 0;
	int mouse_Y = 0;

	bool gun_fired = false;
	bool show_bullet = false;
	bool gun_flash = false;
	int flash_i;
	int flash_angle;
	int fired_time = 0;

	int deviceId = 0;

	int background_size = 2000;
	int player_size = 40;
	int bullet_size = 100;
	int crosshair_size = 80;

	float my_char_angle = 0.f;
	float fired_angle = 0;
	float bullet_angle = 0;

	int flash_sprite_w = 1667;
	int flash_sprite_h = 875;

	int curr_state = 0; // 0: menu, 1: ingame

	string text_in = "";
	char text_show[100]{};
	int text_in_height{ 100 };

	char IPAdress[100];
	char Port[100];
	char Name[100];

	void initVariables();
	void initWindow();

	void clearRenderer();
	void updateRenderer();

	SDL_Texture* loadImage(const char*);
	void loadWavs();
	void loadFont();

	void keyEvent_ingame();
	void mouseEvent_ingame();

	double calcAngleFromPoints(TF first_point, TF second_point);
	int Timer(int start_time, int delay);

	void drawBackground();
	void drawCharacter();
	void drawBullet();
	void drawFlash();
	void drawCrosshair();
	void drawText(int x, int y, char[]);

	void drawMenu();
	void drawIngame();


public:
	float delayTime;
	int done = 0;

	Game();
	virtual ~Game();

	void update();
};