#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"
#include "Bomb.h"

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>

#include <vector>

// Globals

HGE *hge;

Entity * player;
std::vector<Entity *> * enemies;

HTEXTURE playertexture;
HTEXTURE enemytexture1;
HTEXTURE enemytexture2;
HTEXTURE enemytexture3;
HTEXTURE enemytexture4;
HTEXTURE backgroundTexture1;
HTEXTURE backgroundTexture2;
HTEXTURE backgroundTexture3;
HTEXTURE tempboss;
HTEXTURE bombtexture;

hgeFont * hFont;

Level * currentlevel;
std::vector<Level *> levels;

std::vector<Entity *> bombvector;

const int BOMB_TIMEOUT = 1.55;
int bombtimeout = 0;
int bombtimeoutstart = 0;

Level * ChangeLevel(std::vector<Level *> * levels) {
	Level * temp = NULL;

	std::vector<Level *>::iterator iter;
	for(iter = levels->begin(); iter != levels->end(); iter++) {
		temp = *iter;
		if(temp->GetLevelCompleteStatus() == false)
			break;
	}

	return temp;
}

bool FrameFunc() {
	if(hge->Input_GetKeyState(HGEK_ESCAPE)) {
		return true;
	}

	if(currentlevel->GetLevelCompleteStatus() == true) {
		Level * temp = currentlevel;
		currentlevel = ChangeLevel(&levels);

		if(currentlevel == temp)
			return true;

		std::vector<Entity *>::iterator iter;
		for(iter = bombvector.begin(); iter != bombvector.end(); iter++) {
			Entity * temp = *iter;
			if(temp->GetLivingStatus() == true)
				temp->SetLivingStatus(false);
		}

		currentlevel->InitializeLevel();
	}

	if(hge->Input_GetKeyState(HGEK_LBUTTON)) {
		bombtimeout =  hge->Timer_GetTime() - bombtimeoutstart;

		if(bombtimeout >= BOMB_TIMEOUT) {
			bombvector.push_back(new Bomb(hge, bombtexture, hge->Timer_GetTime(), 5, player->GetX(), player->GetY(), 0.0f));
			bombtimeoutstart = hge->Timer_GetTime();
		}
	}

	enemies = currentlevel->GetEnemyVector();

	player->SetEnemyVector(enemies);
	player->Update();  // Get Input and Move the player

	std::vector<Entity *>::iterator bombiterator;
	for(bombiterator = bombvector.begin(); bombiterator != bombvector.end(); bombiterator++) {
		Entity * temp = *bombiterator;

		if(temp->GetLivingStatus() == true) {
			temp->SetEnemyVector(enemies);
			temp->Update();
		}
	}

	std::vector<Entity *>::iterator iter;
	for(iter = enemies->begin(); iter != enemies->end(); iter++) {
		Entity * temp = *iter;
		if(temp->GetLivingStatus() ==  true) {
			temp->Update();
		}
	}

	currentlevel->Update();

	return false;
}

bool RenderFunc() {
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0x000000);

	currentlevel->GetBackgroundSprite()->RenderEx(0.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	std::vector<Entity *>::iterator bombiterator;
	for(bombiterator = bombvector.begin(); bombiterator != bombvector.end(); bombiterator++) {
		Entity * temp = *bombiterator;
		if(temp->GetLivingStatus() == true) {
			temp->GetSprite()->RenderEx(temp->GetX(), temp->GetY(), temp->GetAngle(), 1.0f, 1.0f);
		}
	}

	std::vector<Entity *>::iterator iter;
	for(iter = enemies->begin(); iter != enemies->end(); iter++) {
		Entity * temp = *iter;
		if(temp->GetLivingStatus() == true) {
			temp->GetSprite()->RenderEx(temp->GetX(), temp->GetY(), temp->GetAngle(), 1.0f, 1.0f);
		}
	}

	player->GetSprite()->RenderEx(player->GetX(), player->GetY(), player->GetAngle(), 1.0f, 1.0f);

	hFont->printf(5, 5, HGETEXT_LEFT, "Score: %d", player->GetScore());
	hFont->printf(670, 5, HGETEXT_LEFT, "Health: %.0f%%", (player->GetHP() /  200) * 100);
	hFont->printf(5, 30, HGETEXT_LEFT, "FPS: %d", hge->Timer_GetFPS());

	hge->Gfx_EndScene();
	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);

	hge->System_SetState(HGE_TITLE, "Krazy Kritters - Demo");

	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_HIDEMOUSE, false);

	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate()) {
		hFont = new hgeFont("font.fnt");
		backgroundTexture1 = hge->Texture_Load("background.png");
		backgroundTexture2 = hge->Texture_Load("background2.png");
		backgroundTexture3 = hge->Texture_Load("background3.png");
		enemytexture1 = hge->Texture_Load("enemy_04.png");
		enemytexture2 = hge->Texture_Load("enemy_01.png");
		enemytexture3 = hge->Texture_Load("enemy_03.png");
		enemytexture4 = hge->Texture_Load("enemy_05.png");
		playertexture = hge->Texture_Load("player.png");
		tempboss = hge->Texture_Load("bossbug.png");
		bombtexture = hge->Texture_Load("bomb.png");

		srand(hge->Timer_GetTime());

		player = new Player(playertexture, hge, rand() % 400, rand() % 300, 0, 0, rand() % 360, 1.0);

		ENEMYINFO eibasicenemy = {hge, player, enemytexture1, 2.0f, 10, WANDERER};
		ENEMYINFO eibasicenemy2 = {hge, player, enemytexture3, 1.0f, 15, WANDERER};
		ENEMYINFO eiadvancedenemy = {hge, player, enemytexture2, 1.7f, 1000, PREDATOR};
		ENEMYINFO eiimmuneenemy = {hge, player, enemytexture4, 1.0f, 1000, PREDATOR_IMMUNE};
		ENEMYINFO eiboss1 = {hge, player, tempboss, 1.0f, 6000, BOSS};

		Level * levelmaker;

		levelmaker = new Level(hge, player, backgroundTexture1);
		levelmaker->AddEnemyToLevel(eibasicenemy, 4);
		levelmaker->AddEnemyToLevel(eiadvancedenemy, 1);
		levels.push_back(levelmaker);

		levelmaker = new Level(hge, player, backgroundTexture2);
		levelmaker->AddEnemyToLevel(eibasicenemy, 4);
		levelmaker->AddEnemyToLevel(eiadvancedenemy, 1);
		levelmaker->AddEnemyToLevel(eiimmuneenemy, 2);
		levels.push_back(levelmaker);

		levelmaker = new Level(hge, player, backgroundTexture3);
		levelmaker->AddEnemyToLevel(eibasicenemy, 10);
		levelmaker->AddEnemyToLevel(eibasicenemy2, 3);
		levelmaker->AddEnemyToLevel(eiadvancedenemy, 1);
		levels.push_back(levelmaker);

		levelmaker = new Level(hge, player, backgroundTexture1);
		levelmaker->AddEnemyToLevel(eiboss1, 1);
		levels.push_back(levelmaker);

		currentlevel = ChangeLevel(&levels);

		currentlevel->InitializeLevel();

		hge->System_Start();
	}
	else {
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	hge->System_Shutdown();
	hge->Release();

	return 0;
}