#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"
#include "Bomb.h"
#include "MenuItem.h"

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgegui.h>

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

HEFFECT sound;

HTEXTURE mainmenuscreen;
HTEXTURE gameoverscreen;
HTEXTURE leveltransitionscreen;
HTEXTURE pausescreen;

hgeSprite * mainmenusprite;
hgeSprite * gameoversprite;
hgeSprite * leveltransitionsprite;
hgeSprite * pausesprite;

hgeGUI * mainmenugui;
hgeGUI * gameovergui;
hgeGUI * leveltransitiongui;
hgeGUI * pausegui;

Level * currentlevel;
std::vector<Level *> levels;

std::vector<Entity *> bombvector;

const int BOMB_TIMEOUT = 1.55;
int bombtimeout = 0;
int bombtimeoutstart = 0;

Level * ChangeLevel(std::vector<Level *> * levels);
bool GameFrameFunc();
bool GameRenderFunc();
bool MainMenuFrameFunc();
bool MainMenuRenderFunc();
bool GameOverFrameFunc();
bool GameOverRenderFunc();
bool LevelTransitionFrameFunc();
bool LevelTransitionRenderFunc();
bool PauseFrameFunc();
bool PauseRenderFunc();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	hge = hgeCreate(HGE_VERSION);

	/*hge->System_SetState(HGE_FRAMEFUNC, GameFrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, GameRenderFunc);*/

	hge->System_SetState(HGE_FRAMEFUNC, MainMenuFrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, MainMenuRenderFunc);

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

		sound = hge->Effect_Load("menu.wav");

		mainmenuscreen = hge->Texture_Load("mainmenu.png");
		gameoverscreen = hge->Texture_Load("gameover.png");
		leveltransitionscreen = hge->Texture_Load("levelcomplete.png");
		pausescreen = hge->Texture_Load("pausescreen.png");

		mainmenusprite = new hgeSprite(mainmenuscreen, 0, 0, 800, 600);
		gameoversprite = new hgeSprite(gameoverscreen, 0, 0, 800, 600);
		leveltransitionsprite = new hgeSprite(leveltransitionscreen, 0, 0, 800, 600);
		pausesprite = new hgeSprite(pausescreen, 0, 0, 800, 600);

		mainmenugui = new hgeGUI();
		mainmenugui->AddCtrl(new hgeGUIMenuItem(1, hFont, sound, 400, 200, "New Game or Continue"));
		mainmenugui->AddCtrl(new hgeGUIMenuItem(2, hFont, sound, 400, 250, "Exit Game"));

		gameovergui = new hgeGUI();
		gameovergui->AddCtrl(new hgeGUIMenuItem(1, hFont, sound, 400, 200, "Exit Game"));

		leveltransitiongui = new hgeGUI();
		leveltransitiongui->AddCtrl(new hgeGUIMenuItem(1, hFont, sound, 400, 200, "Start"));

		pausegui = new hgeGUI();
		pausegui->AddCtrl(new hgeGUIMenuItem(1, hFont, sound, 400, 200, "Resume Game"));
		pausegui->AddCtrl(new hgeGUIMenuItem(2, hFont, sound, 400, 250, "Quit Game"));

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
		enemies = currentlevel->GetEnemyVector();

		hge->System_Start();
	}
	else {
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	hge->System_Shutdown();
	hge->Release();

	return 0;
}

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

bool GameFrameFunc() {
	if(hge->Input_GetKeyState(HGEK_ESCAPE)) {
		pausegui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		pausegui->SetFocus(1);
		pausegui->Enter();
		hge->System_SetState(HGE_FRAMEFUNC, PauseFrameFunc);
		hge->System_SetState(HGE_RENDERFUNC, PauseRenderFunc);
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
			bombvector.push_back(new Bomb(hge, bombtexture, hge->Timer_GetTime(), 5, player->GetX()+(cos(player->GetAngle()+1.57)*30), player->GetY()+(sin(player->GetAngle()+1.57) * 30), 0.0f));
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

	/*// Check to see if the player is dead
	if(player->GetHP() <= 0) {
		gameovergui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		gameovergui->SetFocus(1);
		gameovergui->Enter();
		hge->System_SetState(HGE_FRAMEFUNC, GameOverFrameFunc);
		hge->System_SetState(HGE_RENDERFUNC, GameOverRenderFunc);
	}

	// Level complete menu check here
	if(currentlevel->GetLevelCompleteStatus() == true) {
		//switch to menu
		leveltransitiongui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		leveltransitiongui->SetFocus(1);
		leveltransitiongui->Enter();
		hge->System_SetState(HGE_FRAMEFUNC, LevelTransitionFrameFunc);
		hge->System_SetState(HGE_RENDERFUNC, LevelTransitionRenderFunc);
	}*/

	if(currentlevel->GetLevelCompleteByEvent() == 1) {  // Player killed all enemies
		leveltransitiongui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		leveltransitiongui->SetFocus(1);
		leveltransitiongui->Enter();
		hge->System_SetState(HGE_FRAMEFUNC, LevelTransitionFrameFunc);
		hge->System_SetState(HGE_RENDERFUNC, LevelTransitionRenderFunc);
	}
	else if(currentlevel->GetLevelCompleteByEvent() == 2) {  // Player was killed
		gameovergui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
		gameovergui->SetFocus(1);
		gameovergui->Enter();
		hge->System_SetState(HGE_FRAMEFUNC, GameOverFrameFunc);
		hge->System_SetState(HGE_RENDERFUNC, GameOverRenderFunc);
	}

	return false;
}

bool GameRenderFunc() {
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

bool MainMenuFrameFunc() {
	int id;
	static int lastid = 0;
	float deltatime = hge->Timer_GetDelta();

	id = mainmenugui->Update(deltatime);

	if(id == -1) {
		switch(lastid) {
		case 1:
			hge->System_SetState(HGE_FRAMEFUNC, GameFrameFunc);
			hge->System_SetState(HGE_RENDERFUNC, GameRenderFunc);
			break;
		case 2:
			return true;
			break;
		}
	}
	else if(id) {
		lastid = id;
		mainmenugui->Leave();
	}

	return false;
}

bool MainMenuRenderFunc() {
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0x00000000);
	mainmenusprite->RenderEx(0, 0, 0.0f, 1.0f, 1.0f);
	mainmenugui->Render();
	hge->Gfx_EndScene();
	return false;
}

bool GameOverFrameFunc() {
	int id;
	static int lastid = 0;
	float deltatime = hge->Timer_GetDelta();

	id = gameovergui->Update(deltatime);

	if(id == -1) {
		switch(lastid) {
		case 1:
			/*mainmenugui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
			mainmenugui->SetFocus(1);
			mainmenugui->Enter();
			hge->System_SetState(HGE_FRAMEFUNC, MainMenuFrameFunc);
			hge->System_SetState(HGE_RENDERFUNC, MainMenuRenderFunc);*/
			return true;
			break;
		}
	}
	else if(id) {
		lastid = id;
		gameovergui->Leave();
	}

	return false;
}

bool GameOverRenderFunc() {
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0x00000000);
	gameoversprite->RenderEx(0, 0, 0.0f, 1.0f, 1.0f);
	gameovergui->Render();
	hge->Gfx_EndScene();
	return false;
}

bool LevelTransitionFrameFunc() {
	int id;
	static int lastid = 0;
	float deltatime = hge->Timer_GetDelta();

	id = leveltransitiongui->Update(deltatime);

	if(id == -1) {
		switch(lastid) {
		case 1:
			hge->System_SetState(HGE_FRAMEFUNC, GameFrameFunc);
			hge->System_SetState(HGE_RENDERFUNC, GameRenderFunc);
			break;
		}
	}
	else if(id) {
		lastid = id;
		leveltransitiongui->Leave();
	}

	return false;
}

bool LevelTransitionRenderFunc() {
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0x00000000);
	leveltransitionsprite->RenderEx(0, 0, 0.0f, 1.0f, 1.0f);
	leveltransitiongui->Render();
	hge->Gfx_EndScene();
	return false;
}

bool PauseFrameFunc() {
	int id;
	static int lastid = 0;
	float deltatime = hge->Timer_GetDelta();

	id = pausegui->Update(deltatime);

	if(id == -1) {
		switch(lastid) {
		case 1:
			hge->System_SetState(HGE_FRAMEFUNC, GameFrameFunc);
			hge->System_SetState(HGE_RENDERFUNC, GameRenderFunc);
			break;
		case 2:
			mainmenugui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
			mainmenugui->SetFocus(1);
			mainmenugui->Enter();
			hge->System_SetState(HGE_FRAMEFUNC, MainMenuFrameFunc);
			hge->System_SetState(HGE_RENDERFUNC, MainMenuRenderFunc);
			break;
		}
	}
	else if(id) {
		lastid = id;
		pausegui->Leave();
	}

	return false;
}

bool PauseRenderFunc() {
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0x00000000);
	pausesprite->RenderEx(0, 0, 0.0f, 1.0f, 1.0f);
	pausegui->Render();
	hge->Gfx_EndScene();
	return false;
}