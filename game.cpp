#include <iostream>
#include "game.h"

using namespace std;

void Game::initVariables() 
{
	this->window = nullptr;
}
void Game::initWindow() 
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		cout << "Could not initialize SDL!" << SDL_GetError() << endl;
	}
	if (TTF_Init() < 0) {
		cout << "Could not initialize SDL!" << SDL_GetError() << endl;
	}
	
	
	this->iMonitorWidth = GetSystemMetrics(SM_CXSCREEN);    // 모니터 가로 해상도
	this->iMonitorHeight = GetSystemMetrics(SM_CYSCREEN); //새로  해상도

	//Create Window
	this->window = SDL_CreateWindow("", iMonitorWidth / 2 - WIDTH / 2, iMonitorHeight / 2 - HEIGHT / 2, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		cout << "Could not create window!" << SDL_GetError() << endl;;
	}

	// Create renderer
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL) {
		cout << "Could not create renderer!" << SDL_GetError() << endl;
	}
}

SDL_Texture* Game::loadImage(const char* directory)
{
	SDL_Texture* Tex;
	tmpSurface = IMG_Load(directory);
	if (!tmpSurface) {
		cout << "Image not loaded" << directory << endl;
	}
	Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return Tex;
}
void Game::loadWavs()
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	gunsound = Mix_LoadWAV("Sounds/gunsound.wav");
}

double Game::calcAngleFromPoints(TF first_point, TF second_point)
{
	float fAngle;
	float fdX = first_point.x - second_point.x;
	float fdY = first_point.y - second_point.y;
	float dRad = atan2(fdY, fdX);
	return fAngle = (dRad * 180) / 3.14159265;
}

int Game::Timer(int start_time, int delay)
{
	int curr_time = clock();
	if (curr_time - start_time > delay && start_time != 0) {
		//cout << curr_time << " " << start_time << endl;
		return 1;
	}
	else
		return 0;
}

void Game::keyEvent()
{
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT) {
		done = 1;
	}

	//Fix keyboard status when key pressed
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_w) {
			w_Pressed = true;
		}
		if (event.key.keysym.sym == SDLK_a) {
			a_Pressed = true;
		}
		if (event.key.keysym.sym == SDLK_s) {
			s_Pressed = true;
		}
		if (event.key.keysym.sym == SDLK_d) {
			d_Pressed = true;
		}
	}
	if (event.type == SDL_KEYUP) {
		if (event.key.keysym.sym == SDLK_w) {
			w_Pressed = false;
		}
		if (event.key.keysym.sym == SDLK_a) {
			a_Pressed = false;
		}
		if (event.key.keysym.sym == SDLK_s) {
			s_Pressed = false;
		}
		if (event.key.keysym.sym == SDLK_d) {
			d_Pressed = false;
		}
	}

	//Add speed to character by status of key
	float minimun_movement = 5.f * delayTime;
	if (w_Pressed) {
		MyVelo.y -= minimun_movement;
	}
	if (s_Pressed) {
		MyVelo.y += minimun_movement;
	}
	if (a_Pressed) {
		MyVelo.x -= minimun_movement;
	}
	if (d_Pressed) {
		MyVelo.x += minimun_movement;
	}

	float MaxVelo = 200.f * delayTime;
	float MinVelo = -200.f * delayTime;

	//Limit Velocity
	if (MyVelo.x > MaxVelo) {
		MyVelo.x = MaxVelo;
	}
	if (MyVelo.x < MinVelo) {
		MyVelo.x = MinVelo;
	}
	if (MyVelo.y > MaxVelo) {
		MyVelo.y = MaxVelo;
	}
	if (MyVelo.y < MinVelo) {
		MyVelo.y = MinVelo;
	}

	//Character Speed Friction
	float friction = 2.f * delayTime;

	if (MyVelo.x > 0) {
		MyVelo.x -= friction;
	}
	if (MyVelo.x < 0) {
		MyVelo.x += friction;
	}
	if (MyVelo.y > 0) {
		MyVelo.y -= friction;
	}
	if (MyVelo.y < 0) {
		MyVelo.y += friction;
	}

	//offset ignorable velocity
	if (-minimun_movement / 2 <= MyVelo.x && MyVelo.x <= minimun_movement / 2) {
		MyVelo.x = 0;
	}
	if (-minimun_movement / 2 <= MyVelo.y && MyVelo.y <= minimun_movement / 2) {
		MyVelo.y = 0;
	}

	//Keep Character Visible in Window
	if (MyCharPos.x > background_size / 2 - player_size / 2) {
		MyCharPos.x = background_size / 2 - player_size / 2;
		MyVelo.x = 0;
	}
	else if (MyCharPos.x < -background_size / 2 + player_size / 2) {
		MyCharPos.x = -background_size / 2 + player_size / 2;
		MyVelo.x = 0;
	}
	if (MyCharPos.y > background_size / 2 - player_size / 2) {
		MyCharPos.y = background_size / 2 - player_size / 2;
		MyVelo.y = 0;
	}
	else if (MyCharPos.y < -background_size / 2 + player_size / 2) {
		MyCharPos.y = -background_size / 2 + player_size / 2;
		MyVelo.y = 0;
	}
	//add velocity to XY coordinates
	MyCharPos.x += MyVelo.x;
	MyCharPos.y += MyVelo.y;
}
void Game::mouseEvent()
{
	//get mouse coordinates 
	SDL_GetGlobalMouseState(&mouse_X, &mouse_Y);

	mouse_point.x = mouse_X;
	mouse_point.y = mouse_Y;

	//모니터 해상도 좌표를 게임 게임 창 좌표 기준으로 변환시킴
	SDL_GetWindowPosition(window, &window_moved_x, &window_moved_y);
	mouse_point.x -= window_moved_x;
	mouse_point.y -= window_moved_y;

	//Get Character Angle by Mouse's Coordinates
	my_char_angle = calcAngleFromPoints(mouse_point, middle_pos);

	//마우스 왼 버튼 누르면 발사
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT && gun_fired == false) {
			gun_fired = true;
			fired_time = clock();

			show_bullet = true;
			gun_flash = true;

			Mix_PlayChannel(-1, gunsound, 0);
		}
	}
	//총이 발사되고 타이머 작동시켜서 1초 뒤 다시 발사 가능
	if (gun_fired) {
		if (Timer(fired_time, 100) == 1) {
			gun_fired = false;
			fired_time = 0;
		}
	}
}

void Game::drawBackground()
{
	destR.w = background_size;
	destR.h = background_size;
	destR.x = WIDTH / 2 - background_size / 2 - MyCharPos.x;
	destR.y = HEIGHT / 2 - background_size / 2 - MyCharPos.y;

	//cout << MyCharPos.x << " " << MyCharPos.y << endl;

	//Draw Background
	SDL_RenderCopy(renderer, backTex, NULL, &destR);
}
void Game::drawCharacter()
{
	//// Draw rect
	//SDL_Rect r = { MyChar_X, MyChar_Y, rect_size, rect_size };
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderDrawRect(renderer, &r);

	destR.w = player_size;
	destR.h = player_size;
	destR.x = WIDTH / 2 - player_size / 2; //MyCharPos.x - player_size / 2;
	destR.y = HEIGHT / 2 - player_size / 2; //MyCharPos.y - player_size / 2;

	center.x = player_size / 2;
	center.y = player_size / 2;

	SDL_RenderCopyEx(renderer, playerTex, NULL, &destR, my_char_angle + 90, &center, SDL_FLIP_NONE);
}
void Game::drawBullet()
{
	bullet_angle = 3.14159265 * 2 * fired_angle / 360;
	//총알이 보일 때
	if (show_bullet) {

		myBulletVelo.x = cos(bullet_angle) * 6000 * delayTime;
		myBulletVelo.y = sin(bullet_angle) * 6000 * delayTime;

		myBulletPos.x += (myBulletVelo.x + MyVelo.x);
		myBulletPos.y += (myBulletVelo.y + MyVelo.y);

		destR.w = bullet_size;
		destR.h = bullet_size;
		destR.x = myBulletPos.x - bullet_size / 2;
		destR.y = myBulletPos.y - bullet_size / 2;

		center.x = bullet_size / 2;
		center.y = bullet_size / 2;

		//SDL_RenderCopy(renderer, bulletTex, NULL, &destR);
		SDL_RenderCopyEx(renderer, bulletTex, NULL, &destR, fired_angle, &center, SDL_FLIP_NONE);

		if (myBulletPos.x > WIDTH || myBulletPos.x < 0 || myBulletPos.y >HEIGHT || myBulletPos.y < 0) {
			show_bullet = false;
		}
	}
	//총알이 보이지 않을 때
	else {
		myBulletPos.x = WIDTH / 2 + cos(bullet_angle) * 75;
		myBulletPos.y = HEIGHT / 2 + sin(bullet_angle) * 75;
		//캐릭터가 발사한 순간의 각도를 
		fired_angle = my_char_angle;
	}
}
void Game::drawFlash()
{
	flash_size.x = 150;
	flash_size.y = 80;

	if (gun_flash) {
		srcR.w = flash_sprite_w / 4;
		srcR.h = flash_sprite_h / 3;

		flash_i += 1;
		if (flash_i % 2 == 0) {
			srcR.x += srcR.w;
			srcR.y = 320;
		}

		if (srcR.x >= flash_sprite_w - flash_sprite_w % 4) {
			srcR.x = 0;

			flash_i = 0;
			gun_flash = false;
		}

		destR.w = flash_size.x;
		destR.h = flash_size.y;
		destR.x = WIDTH / 2 - flash_size.x / 2 + cos(bullet_angle) * 75;
		destR.y = HEIGHT / 2 - flash_size.y / 2 + sin(bullet_angle) * 75;

		center.x = flash_size.x / 2;
		center.y = flash_size.y / 2;

		SDL_RenderCopyEx(renderer, flashTex, &srcR, &destR, fired_angle, &center, SDL_FLIP_NONE);
	}
	else {
		flash_angle = my_char_angle;
	}
}
void Game::drawCrosshair()
{
	//마우스 좌표 먹이기
	destR.w = crosshair_size;
	destR.h = crosshair_size;
	destR.x = mouse_point.x - crosshair_size / 2;
	destR.y = mouse_point.y - crosshair_size / 2;

	//Draw Target
	SDL_RenderCopy(renderer, targetTex, NULL, &destR);
	//SDL_RenderCopyEx(renderer, targetTex, NULL, &destR, my_char_angle, &center, SDL_FLIP_NONE);
}

void Game::clearRenderer()
{
	// Clear renderer
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}
void Game::updateRenderer()
{
	// Update screen
	SDL_RenderPresent(renderer);
}

void Game::drawMenu()
{
	TTF_Font* font = TTF_OpenFont("arial.ttf", 16);
	if (font == NULL) {
		printf("Could not open font! (%s)\n", TTF_GetError());
	}

	SDL_Color color = { 255, 255, 255, SDL_ALPHA_OPAQUE };
	SDL_Surface* surface = TTF_RenderText_Blended(font, "===Main===", color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	
	SDL_Rect r = { 0, 0, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &r);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
}
void Game::drawLobby()
{
	
}
void Game::drawIngame()
{
	drawBackground();
	drawBullet();
	drawFlash();
	drawCharacter();
	drawCrosshair();
}



void Game::update() 
{
	clearRenderer();
	
	mouseEvent();
	keyEvent();
	
	if (curr_state == 0) 
	{
		drawMenu();
	}
	else if (curr_state == 1)
	{
		drawLobby();
	}
	else if (curr_state == 2)
	{
		drawIngame();
	}
	

	updateRenderer();
}

Game::Game()
{
	this->initVariables();
	this->initWindow();

	playerTex = this->loadImage("Images/Player.png");
	targetTex = this->loadImage("Images/Target.png");
	backTex = this->loadImage("Images/Background.jpg");
	bulletTex = this->loadImage("Images/Bullet.png");
	flashTex = this->loadImage("Images/Flash.png");

	this->loadWavs();

	//Initialize player's position to middle of window
	MyCharPos.x = 0;
	MyCharPos.y = 0;

	middle_pos.x = WIDTH / 2;
	middle_pos.y = HEIGHT / 2;
}

Game::~Game()
{
	Mix_FreeChunk(gunsound);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}