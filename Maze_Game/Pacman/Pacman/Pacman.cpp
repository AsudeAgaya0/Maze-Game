//-----------------------------------------------------------------
// Fore 2 Application
// C++ Source - Fore.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Pacman.h"
#include<iostream>
#include<fstream>
#include <list>


extern Bitmap* _pHeartBitmap;
extern Bitmap* _pWizardHatBitmap;
extern Bitmap* _pKeyScoreBitmap;
extern Bitmap* _pLiveBoostBitmap;
extern Bitmap* _pMineBitmap;


//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
	// Create the game engine
	_pGame = new GameEngine(hInstance, LPTSTR(TEXT("Pacman")),
		LPTSTR(TEXT("Pacman")), IDI_PACMAN, IDI_PACMAN_SM, 55 * 25, 30 * 25);
	if (_pGame == NULL)
		return FALSE;

	// Set the frame rate
	_pGame->SetFrameRate(30);

	// Store the instance handle
	_hInstance = hInstance;

	return TRUE;
}

void GameStart(HWND hWindow)
{
	// Seed the random number generator
	srand(GetTickCount());

	// Create the offscreen device context and bitmap
	_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
	_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
		_pGame->GetWidth(), _pGame->GetHeight());
	SelectObject(_hOffscreenDC, _hOffscreenBitmap);

	// Create and load the bitmaps
	HDC hDC = GetDC(hWindow);
	_pDoorBitmap = new Bitmap(hDC, IDB_DOOR, _hInstance);
	_pWallBitmap = new Bitmap(hDC, IDB_WALL, _hInstance);
	_pPortalBitmap = new Bitmap(hDC, IDB_PORTAL, _hInstance);
	_pPacmanBitmap = new Bitmap(hDC, IDB_PACMAN, _hInstance);
	_pDotBitmap = new Bitmap(hDC, IDB_DOT, _hInstance);
	_pSmDotBitmap = new Bitmap(hDC, IDB_SM_DOT, _hInstance);
	_pIceGunBitmap = new Bitmap(hDC, IDB_ICE_GUN, _hInstance);
	_pGhostBitmap = new Bitmap(hDC, IDB_GHOST, _hInstance);
	_pWizardBitmap = new Bitmap(hDC, IDB_WIZARD, _hInstance);
	_pLgExplosionBitmap = new Bitmap(hDC, IDB_LGEXPLOSION, _hInstance);

	_pHeartBitmap = new Bitmap(hDC, IDB_HEART, _hInstance);
	_pWizardHatBitmap = new Bitmap(hDC, IDB_WIZARDHAT, _hInstance);
	_pKeyScoreBitmap = new Bitmap(hDC, IDB_KEYSCORE, _hInstance);
	_pLiveBoostBitmap = new Bitmap(hDC, IDB_BOOST, _hInstance);

	_pGhost1Bitmap = new Bitmap(hDC, IDB_GHOST1, _hInstance);
	_pCandyBitmap = new Bitmap(hDC, IDB_CANDY, _hInstance);
	_pMineBitmap = new Bitmap(hDC, IDB_MINE, _hInstance);

	// Create the background
	_pBackground = new Background(55 * 25, 30 * 25, RGB(0, 0, 0));



	path = new coord * [1000];
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < maxrow; ++j) {
			for (int k = 0; k < maxcol; ++k) {

				maze3d[i][j][k] = 0;

				if (j == 0 || j == maxrow - 1 || k == 0 || k == maxcol - 1) {//corner walls
					maze3d[i][j][k] = 100;
				}
				if (j % 2 == 0 && k % 2 == 0) {//walls
					maze3d[i][j][k] = 100;
				}
				if (i == 1 && ((j == 10 && k == 13) || (j == 11 && k == 12) || (j == 11 && k == 14))) {//door walls
					maze3d[i][j][k] = 100;
				}
				if (i == 1 && j == 12 && k == 13) {//door
					maze3d[i][j][k] = 102;
				}
				if (i == 1 && j == 11 && k == 13) {//wizard
					maze3d[i][j][k] = 201;
				}
				if (i == 0 && j == 11 && k == 12) {//pacman
					maze3d[i][j][k] = 200;
					pacmanLoc = 0;
				}
			}
		}
	}
	NewGame();

	//create random portals
	int portalCount = 4;
	while (true) {
		int j = (rand() % (maxcol - 2)) + 1;
		int k = (rand() % (maxrow - 2)) + 1;

		if (maze3d[0][j][k] < 100 && maze3d[1][j][k] < 100) {
			maze3d[0][j][k] = 202;
			maze3d[1][j][k] = 202;
			portalCount--;
		}
		if (portalCount == 0) break;
	}
	//create
	int keyCount = 13;
	while (true) {
		int i = (rand() % 2);
		int j = (rand() % (maxcol - 2)) + 1;
		int k = (rand() % (maxrow - 2)) + 1;

		if (maze3d[i][j][k] < 100) {
			maze3d[i][j][k] = 2;
			keyCount--;
		}
		if (keyCount == 0) break;
	}


	//store temp maze for additional walls
	int** pam;
	pam = new int* [maxrow];
	for (int i = 0; i < maxrow; i++) pam[i] = new int[maxcol];
	int** pam1;
	pam1 = new int* [maxrow];
	for (int i = 0; i < maxrow; i++) pam1[i] = new int[maxcol];

	for (int y = 0; y < maxrow; y++) {
		for (int x = 0; x < maxcol; x++) {
			pam[y][x] = maze3d[0][y][x];
			pam1[y][x] = maze3d[1][y][x];
		}
	}
	//additonal walls dor maze3d[0] and maze3d[1]
	addWall(pam, 0);
	addWall(pam1, 1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void addWall(int** pam, int index) {
	for (wall = 0; wall < maxwall * 0.6; wall++) {//wall sayısı oranı
		int x, y, x1, y1, x2, y2; // x1, y1 -> x2, y2 access?
		bool wallok;
		coord* c1, * c2;
		wallok = false;

		do {
			x = (rand() % (maxcol - 2)) + 1;
			y = (rand() % (maxrow - 2)) + 1;
			if ((x % 2 == 0 && y % 2 == 1) || (x % 2 == 1 && y % 2 == 0)) {
				if ((x % 2 == 0 && y % 2 == 1)) {
					x1 = x - 1; x2 = x + 1;
					y1 = y2 = y;
				}
				if ((x % 2 == 1 && y % 2 == 0)) {
					x1 = x2 = x;
					y1 = y - 1; y2 = y + 1;
				}
				if (pam[y][x] == 0) {
					pam[y][x] = 101;
					wallok = true;

					c1 = new coord(x1, y1);
					c2 = new coord(x2, y2);
					if (findPathBFS(c1, c2, pam, maxrow, maxcol, 0, path) < 0) {
						pam[y][x] = 0;
						wallok = false;
					}
				}

			}

		} while (wallok == false);

	}

	//update real maze
	for (int y = 0; y < maxrow; y++) {
		for (int x = 0; x < maxcol; x++) {
			maze3d[index][y][x] = pam[y][x];
		}
	}
}

int distance(coord* c1, coord* c2) {
	int dx, dy;
	dx = abs(c1->x - c2->x);
	dy = abs(c1->y - c2->y);
	return dx + dy;
}

int findPathBFS(coord* c1, coord* c2, int** map, int naxrow, int  maxcol, int empty, coord** path) {
	int** map2;
	queue<coord*> alt;
	coord* c, * ct;

	//create spare map
	map2 = new int* [maxrow];
	for (int i = 0; i < maxrow; i++) map2[i] = new int[maxcol];

	for (int y = 0; y < maxrow; y++) {
		for (int x = 0; x < maxcol; x++) {
			map2[y][x] = map[y][x];
		}
	}

	alt.push(c1);// beginning
	while (!alt.empty()) {
		c = alt.front(); alt.pop();
		if (map2[c->y][c->x] != empty) continue;
		map2[c->y][c->x] = 1; //tested

		if (distance(c, c2) <= 1) {//duvara gidemez
			int i = 0;
			while (c->prev != NULL) {
				path[i] = c;
				c = c->prev;
				i++;
			}
			return i;
		}

		if (c->x + 1 < maxcol && map2[c->y][c->x + 1] == empty) alt.push(new coord(c->x + 1, c->y, c->dist + 1, c));
		if (c->x - 1 >= 0 && map2[c->y][c->x - 1] == empty)alt.push(new coord(c->x - 1, c->y, c->dist + 1, c));
		if (c->y + 1 < maxcol && map2[c->y + 1][c->x] == empty) alt.push(new coord(c->x, c->y + 1, c->dist + 1, c));
		if (c->y - 1 >= 0 && map2[c->y - 1][c->x] == empty) alt.push(new coord(c->x, c->y - 1, c->dist + 1, c));

	}
	return -1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameEnd()
{
	// Cleanup the offscreen device context and bitmap
	DeleteObject(_hOffscreenBitmap);
	DeleteDC(_hOffscreenDC);

	// Cleanup the bitmaps
	delete _pDoorBitmap;
	delete _pWallBitmap;
	delete _pPortalBitmap;
	delete _pPacmanBitmap;
	delete _pDotBitmap;
	delete _pSmDotBitmap;
	delete _pIceGunBitmap;
	delete _pGhostBitmap;
	delete _pWizardBitmap;
	delete _pHeartBitmap;
	delete _pWizardHatBitmap;
	delete _pKeyScoreBitmap;

	delete _pGhost1Bitmap;
	delete _pCandyBitmap;
	delete _pMineBitmap;


	// Cleanup the background
	delete _pBackground;

	// Cleanup the sprites
	_pGame->CleanupSprites();

	// Cleanup the game engine
	delete _pGame;
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
	// Draw the background
	_pBackground->Draw(hDC);


	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < maxrow; ++j) {
			for (int k = 0; k < maxcol; ++k) {

				if (maze3d[i][j][k] == 100 || maze3d[i][j][k] == 101) {
					if (i == 0) {
						int row = (j * 25) + (4 * 25);
						int column = (k * 25) + (1 * 25);
						_pWallBitmap->Draw(hDC, column, row);
					}
					else {
						int row = (j * 25) + (4 * 25);
						int column = (k * 25) + (29 * 25);
						_pWallBitmap->Draw(hDC, column, row);
					}
				}
				if (maze3d[i][j][k] == 2) {//key
					if (i == 0) {
						int row = (j * 25) + (4 * 25);
						int column = (k * 25) + (1 * 25);
						_pDotBitmap->Draw(hDC, column, row);
					}
					else {
						int row = (j * 25) + (4 * 25);
						int column = (k * 25) + (29 * 25);
						_pDotBitmap->Draw(hDC, column, row);
					}
				}

				if (maze3d[i][j][k] == 102) {//door
					int row = (j * 25) + (4 * 25);
					int column = (k * 25) + (29 * 25);
					_pDoorBitmap->Draw(hDC, column, row);

				}
				if (maze3d[i][j][k] == 201) { //wizard
					int row = (j * 25) + (4 * 25);
					int column = (k * 25) + (29 * 25);
					_pWizardBitmap->Draw(hDC, column, row);

				}
				if (maze3d[i][j][k] == 202) { //portal
					if (i == 0) {
						int row = (j * 25) + (4 * 25);
						int column = (k * 25) + (1 * 25);
						_pPortalBitmap->Draw(hDC, column, row);
					}
					else {
						int row = (j * 25) + (4 * 25);
						int column = (k * 25) + (29 * 25);
						_pPortalBitmap->Draw(hDC, column, row);
					}
				}

				/*if (maze3d[i][j][k] == 205) { //boost
					if (i == 0) {
						int row = (j * 25) + (4 * 25);
						int column = (k * 25) + (1 * 25);
						_pCandyBitmap->Draw(hDC, column, row);
					}
					else {
						int row = (j * 25) + (4 * 25);
						int column = (k * 25) + (29 * 25);
						_pCandyBitmap->Draw(hDC, column, row);

					}

				}
				if (maze3d[i][j][k] == 206) { //mine
					if (i == 0) {
						int row = (j * 25) + (4 * 25);
						int column = (k * 25) + (1 * 25);
						_pMineBitmap->Draw(hDC, column, row);

					}
					else {
						int row = (j * 25) + (4 * 25);
						int column = (k * 25) + (29 * 25);
						_pMineBitmap->Draw(hDC, column, row);

					}


				}*/
			}
		}
	}
	// Draw the sprites
	_pGame->DrawSprites(hDC);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw the number of remaining lives (heart)
	for (int i = 0; i < _iNumLives; i++)
		_pHeartBitmap->Draw(hDC, 50 + (_pHeartBitmap->GetWidth() * i), 50, TRUE);//520 10


		// Draw the number of remaining lives (wizard hat)
	for (int i = 0; i < _iWizardhat; i++)
		_pWizardHatBitmap->Draw(hDC, 300 + (_pWizardHatBitmap->GetWidth() * i), 50, TRUE);//520 10

	// Draw the number of remaining lives (key score)
	for (int i = 0; i < _iNumKeys; i++)
		_pDotBitmap->Draw(hDC, 800 + (_pDotBitmap->GetWidth() * i), 50, TRUE);//520 10


	// Draw the score
	TCHAR szText[64];
	RECT  rect = { 0, 0, 50, 30 };
	wsprintf(szText, "%d", _iScore);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	rect = { 0, 0, 100, 30 };
	wsprintf(szText, "%d", _iNumKeys);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void GameCycle()
{
	gametime++;
	if (!_bGameOver)
	{
		if (!_bGhostDead && gametime % 6 == 0) {
			GhostMoveXY(_pPacManSprite, ghostSpritesXY);
			//MineBoostMoveXY(mineSprites, _pPacManSprite);
			int _iRandom = rand() % 100;
			if (_iRandom < 80) {
				GhostMove(ghostSprites);
			}
		}
		if (!_bGhostDead && gametime % 12 == 0) {
			int _iRandom = rand() % 100;
			if (_iRandom < 60) {
				GhostMove(ghostSprites);
			}
		}

		// Update the background
		_pBackground->Update();
		// Update the sprites
		_pGame->UpdateSprites();

		// Obtain a device context for repainting the game
		HWND  hWindow = _pGame->GetWindow();
		HDC   hDC = GetDC(hWindow);

		// Paint the game to the offscreen device context
		GamePaint(_hOffscreenDC);

		// Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),
			_hOffscreenDC, 0, 0, SRCCOPY);

		// Cleanup
		ReleaseDC(hWindow, hDC);
	}

	// Start a new game based upon an Enter (Return) key press
	if (_bGameOver && (GetAsyncKeyState(VK_RETURN) < 0)) {
		// Start a new game
		NewGame();
	}
}

void NewGame()
{
	// Clear the sprites
	_pGame->CleanupSprites();

	// Create the car sprite
	RECT rcBounds = { 0, 0, 55 * 25, 30 * 25 };
	_pPacManSprite = new Sprite(_pPacmanBitmap, rcBounds, BA_WRAP);
	_pPacManSprite->SetPosition(13 * 25, 15 * 25);
	_pGame->AddSprite(_pPacManSprite);
	_iScore = 0;
	_iNumKeys = 0;
	_iNumLives = 6;
	_bGameOver = FALSE;

	_iWizardhat = 1;

	_iKeyScore = 5;

	AddGhost();
	AddBoost();
	//AddMine();

}

void HandleKeys()
{
	if (!_bGameOver && gametime % 1 == 0)
	{
		//maze3d[0][11][12] = 0;
		POINT ptVelocity = _pPacManSprite->GetVelocity();
		RECT  m_rcPosition = _pPacManSprite->GetPosition();
		if (GetAsyncKeyState(VK_LEFT) < 0)
		{
			_iDirection = 0;
			if (pacmanLoc == 0) {
				if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 2] != 100 &&
					maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 2] != 101) {

					if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 2] == 0 ||
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 2] == 1) {
						if(maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1]!=202) maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						m_rcPosition.left = m_rcPosition.left - 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 2] == 2) {//key
						_iScore += 50;
						_iNumKeys += 1;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						m_rcPosition.left = m_rcPosition.left - 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					}

					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 2] == 206) {//mine
						_iNumLives--;
						pacmanLoc = 0;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 2] = 1;// old pos
						_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

					}

					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 2] == 202) {//next step portal
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						int wx = (m_rcPosition.left / 25) - 2;
						int wy = (m_rcPosition.top / 25) - 4;
						m_rcPosition.left = m_rcPosition.left + (27 * 25);
						_pPacManSprite->SetPosition(m_rcPosition);
						pacmanLoc = 1;
						maze3d[0][wy][wx] = 202;
						maze3d[1][wy][wx] = 202;
					}
					//if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] == 202) {//current step portal
					//	m_rcPosition.left = m_rcPosition.left - 25;
					//	_pPacManSprite->SetPosition(m_rcPosition);
					//	//RECT  newPos = _pPacManSprite->GetPosition();
					//	maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					//}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 2] == 205) {//boost
						_iScore += 50;
						_iNumLives += 1;
						_pCandySprite->Kill();
						//_pGame->_pCandySprite(;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 2] = 0;
					}
				}
			}
			else if (pacmanLoc == 1) {
				if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 30] != 100 &&
					maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 30] != 101) {

					if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 30] == 0 ||
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 30] == 1) {
						if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] != 202) maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						m_rcPosition.left = m_rcPosition.left - 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 30] == 2) {//key
						_iScore += 50;
						_iNumKeys += 1;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						m_rcPosition.left = m_rcPosition.left - 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					}

					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 30] == 206) {//mine
						_iNumLives--;
						pacmanLoc = 0;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 30] = 1;// old pos
						_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

					}

					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 30] == 202) {//next step portal
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						
						int wx = (m_rcPosition.left / 25) - 30;
						int wy = (m_rcPosition.top / 25) - 4;
						m_rcPosition.left = m_rcPosition.left - (29 * 25);
						_pPacManSprite->SetPosition(m_rcPosition);
						pacmanLoc = 0;
						maze3d[0][wy][wx] = 202;
						maze3d[1][wy][wx] = 202;

					}
					//if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] == 202) {//current step portal
					//	m_rcPosition.left = m_rcPosition.left - 25;
					//	_pPacManSprite->SetPosition(m_rcPosition);
					//	//RECT  newPos = _pPacManSprite->GetPosition();
					//	maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					//}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 30] == 205) {//boost
						_iScore += 50;
						_iNumLives += 1;
						_pCandySprite->Kill();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 30] = 0;
					}
				}
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT) < 0)
		{
			_iDirection = 1;
			if (pacmanLoc == 0) {
				if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25)] != 100 &&
					maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25)] != 101) {

					if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25)] == 0 ||
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25)] == 1) {
						if(maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1]!=202) maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						m_rcPosition.left = m_rcPosition.left + 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25)] == 2) {//key
						_iScore += 50;
						_iNumKeys += 1;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						m_rcPosition.left = m_rcPosition.left + 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25)] == 206) {//mine
						_iNumLives--;
						pacmanLoc = 0;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25)] = 1;// old pos
						_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25)] == 202) {//next step portal
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
					
						int wx = (m_rcPosition.left / 25);
						int wy = (m_rcPosition.top / 25) - 4;
						m_rcPosition.left = m_rcPosition.left + (29 * 25);
						_pPacManSprite->SetPosition(m_rcPosition);
						pacmanLoc = 1;
						maze3d[0][wy][wx] = 202;
						maze3d[1][wy][wx] = 202;

					}
					//if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] == 202) {//current step portal
					//	m_rcPosition.left = m_rcPosition.left + 25;
					//	_pPacManSprite->SetPosition(m_rcPosition);
					//	//RECT  newPos = _pPacManSprite->GetPosition();
					//	//maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					//}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25)] == 205) {//boost
						_iScore += 50;
						_iNumLives += 1;
						_pCandySprite->Kill();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25)] = 0;
					}
				}
			}
			else if (pacmanLoc == 1) {
				if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 28] != 100 &&
					maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 28] != 101) {

					if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 28] == 0 ||
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 28] == 1) {
						if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] != 202) maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						m_rcPosition.left = m_rcPosition.left + 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 28] == 2) {//key
						_iScore += 50;
						_iNumKeys += 1;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						m_rcPosition.left = m_rcPosition.left + 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 28] == 206) {//mine
						_iNumLives--;
						pacmanLoc = 0;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 28] = 1;// old pos
						_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

					}

					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 28] == 202) {//next step portal
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						
						int wx = (m_rcPosition.left / 25) - 28;
						int wy = (m_rcPosition.top / 25) - 4;
						m_rcPosition.left = m_rcPosition.left - (27 * 25);
						_pPacManSprite->SetPosition(m_rcPosition);
						pacmanLoc = 0;
						maze3d[0][wy][wx] = 202;
						maze3d[1][wy][wx] = 202;

					}
					//if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] == 202) {//current step portal
					//	m_rcPosition.left = m_rcPosition.left + 25;
					//	_pPacManSprite->SetPosition(m_rcPosition);
					//	//RECT  newPos = _pPacManSprite->GetPosition();
					//	maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					//}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 28] == 205) {//boost
						_iScore += 50;
						_iNumLives += 1;
						_pCandySprite->Kill();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 28] = 0;
					}
				}
			}
		}
		else if (GetAsyncKeyState(VK_UP) < 0)
		{
			_iDirection = 2;
			if (pacmanLoc == 0) {
				if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 1] != 100 &&
					maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 1] != 101) {

					if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 1] == 0 ||
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 1] == 1) {
						if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] != 202) maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						m_rcPosition.top = m_rcPosition.top - 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 1] == 2) {//key
						_iScore += 50;
						_iNumKeys += 1;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						m_rcPosition.top = m_rcPosition.top - 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 1] == 206) {//mine
						_iNumLives--;
						pacmanLoc = 0;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 1] = 1;// old pos
						_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 1] == 202) {//next step portal
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						
						int wx = (m_rcPosition.left / 25) - 1;
						int wy = (m_rcPosition.top / 25) - 5;
						m_rcPosition.top = m_rcPosition.top - (25 * 1);
						m_rcPosition.left = m_rcPosition.left + (28 * 25);
						_pPacManSprite->SetPosition(m_rcPosition);
						pacmanLoc = 1;
						maze3d[0][wy][wx] = 202;
						maze3d[1][wy][wx] = 202;

					}
					//if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] == 202) {//current step portal
					//	m_rcPosition.top = m_rcPosition.top - 25;
					//	_pPacManSprite->SetPosition(m_rcPosition);
					//	//RECT  newPos = _pPacManSprite->GetPosition();
					//	maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					//}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 1] == 205) {//boost
						_iScore += 50;
						_iNumLives += 1;
						_pCandySprite->Kill();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 1] = 0;
					}
				}
			}
			else if (pacmanLoc == 1) {
				if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] != 100 &&
					maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] != 101) {

					if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] == 0 ||
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] == 1) {
						if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] != 202) maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						m_rcPosition.top = m_rcPosition.top - 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] == 2) {//key
						_iScore += 50;
						_iNumKeys += 1;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						m_rcPosition.top = m_rcPosition.top - 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] == 206) {//mine
						_iNumLives--;
						pacmanLoc = 0;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] = 1;// old pos
						_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] == 102 && _iNumKeys == 10) {//door
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] = 0;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						m_rcPosition.top = m_rcPosition.top - 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] == 201) {//wizard
						_bGameOver = TRUE;
					}

					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] == 202) {//next step portal
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						
						int wx = (m_rcPosition.left / 25) - 29;
						int wy = (m_rcPosition.top / 25) - 5;
						m_rcPosition.top = m_rcPosition.top - (25 * 1);
						m_rcPosition.left = m_rcPosition.left - (28 * 25);
						_pPacManSprite->SetPosition(m_rcPosition);
						pacmanLoc = 0;
						maze3d[0][wy][wx] = 202;
						maze3d[1][wy][wx] = 202;

					}
					//if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] == 202) {//current step portal
					//	m_rcPosition.top = m_rcPosition.top - 25;
					//	_pPacManSprite->SetPosition(m_rcPosition);
					//	//RECT  newPos = _pPacManSprite->GetPosition();
					//	maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					//}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] == 205) {//boost
						_iScore += 50;
						_iNumLives += 1;
						_pCandySprite->Kill();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 5][(m_rcPosition.left / 25) - 29] = 0;
					}
				}
			}
		}
		else if (GetAsyncKeyState(VK_DOWN) < 0)
		{
			_iDirection = 3;
			if (pacmanLoc == 0) {
				if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 1] != 100 &&
					maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 1] != 101) {

					if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 1] == 0 ||
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 1] == 1) {
						if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] != 202) maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						m_rcPosition.top = m_rcPosition.top + 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 1] == 2) {//key
						_iScore += 50;
						_iNumKeys += 1;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						m_rcPosition.top = m_rcPosition.top + 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 1] == 206) {//mine
						_iNumLives--;
						pacmanLoc = 0;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 1] = 1;// old pos
						_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 1] == 202) {//next step portal
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 1;// old pos
						
						int wx = (m_rcPosition.left / 25) - 1;
						int wy = (m_rcPosition.top / 25) - 3;
						m_rcPosition.top = m_rcPosition.top + (25 * 1);
						m_rcPosition.left = m_rcPosition.left + (28 * 25);
						_pPacManSprite->SetPosition(m_rcPosition);
						pacmanLoc = 1;
						maze3d[0][wy][wx] = 202;
						maze3d[1][wy][wx] = 202;

					}
					//if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] == 202) {//current step portal
					//	m_rcPosition.top = m_rcPosition.top + 25;
					//	_pPacManSprite->SetPosition(m_rcPosition);
					//	//RECT  newPos = _pPacManSprite->GetPosition();
					//	maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 1] = 200;//new pos
					//}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 1] == 2) {//boost
						_iScore += 50;
						_iNumLives += 1;
						_pCandySprite->Kill();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 1] = 0;
					}
				}
			}
			else if (pacmanLoc == 1) {
				if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 29] != 100 &&
					maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 29] != 101) {

					if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 29] == 0 ||
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 29] == 1) {
						if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] != 202) maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						m_rcPosition.top = m_rcPosition.top + 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 29] == 2) {//key
						_iScore += 50;
						_iNumKeys += 1;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
						m_rcPosition.top = m_rcPosition.top + 25;
						_pPacManSprite->SetPosition(m_rcPosition);
						//RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 29] == 206) {//mine
						_iNumLives--;
						pacmanLoc = 0;
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 29] = 1;// old pos
						_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						RECT  newPos = _pPacManSprite->GetPosition();
						maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

					}

					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 29] == 202) {//next step portal
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 1;// old pos
					
						int wx = (m_rcPosition.left / 25) - 29;
						int wy = (m_rcPosition.top / 25) - 3;
						m_rcPosition.top = m_rcPosition.top + (25 * 1);
						m_rcPosition.left = m_rcPosition.left - (28 * 25);
						_pPacManSprite->SetPosition(m_rcPosition);
						pacmanLoc = 0;
						maze3d[0][wy][wx] = 202;
						maze3d[1][wy][wx] = 202;

					}
					//if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] == 202) {//current step portal
					//	m_rcPosition.top = m_rcPosition.top + 25;
					//	_pPacManSprite->SetPosition(m_rcPosition);
					//	//RECT  newPos = _pPacManSprite->GetPosition();
					//	maze3d[pacmanLoc][(m_rcPosition.top / 25) - 4][(m_rcPosition.left / 25) - 29] = 200;//new pos
					//}
					else if (maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 29] == 205) {//boost
						_iScore += 50;
						_iNumLives += 1;
						_pCandySprite->Kill();
						maze3d[pacmanLoc][(m_rcPosition.top / 25) - 3][(m_rcPosition.left / 25) - 29] = 0;
					}
				}
			}
		}
		if (_iNumLives == 0) _bGameOver = TRUE;

	}
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
	return FALSE;
}

void SpriteDying(Sprite* pSprite)
{
}

void AddGhost()
{
	// Create a new random ghost sprite
	RECT rcBounds1 = { 1 * 25, 5 * 25, 26 * 25, 30 * 25 };
	RECT rcBounds2 = { 29 * 25, 5 * 25, 55 * 25, 30 * 25 };

	int rgcount = 0;
	int xygcount = 0;

	//Random move ghost
	while (rgcount != numGhost)
	{
		if (ghostLoc == 0) {
			_pGhostSpriteR = new Sprite(_pGhostBitmap, rcBounds1, BA_BOUNCE);
			int xpos = ((rand() % 25) + 1) * 25;
			int ypos = ((rand() % 25) + 4) * 25;
			//_pGhostSpriteR->SetPosition(((rand() % 25) + 1) * 25, ((rand() % 25) + 4) * 25);
			//RECT  rcPos = _pGhostSpriteR->GetPosition();
			if (maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 1] < 100 && maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 1] != 2) {
				_pGhostSpriteR->SetPosition(xpos, ypos);
				ghostSprites[rgcount] = _pGhostSpriteR;
				ghostLocs[rgcount] = ghostLoc;
				_pGame->AddSprite(ghostSprites[rgcount]);
				maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 1] = 203;
				ghostLoc = 1;
				rgcount++;
			}
		}
		else if (ghostLoc == 1) {
			_pGhostSpriteR = new Sprite(_pGhostBitmap, rcBounds2, BA_BOUNCE);
			int xpos = ((rand() % 25) + 29) * 25;
			int ypos = ((rand() % 25) + 4) * 25;
			//_pGhostSpriteR->SetPosition(((rand() % 25) + 29) * 25, ((rand() % 25) + 4) * 25);
			//RECT  rcPos = _pGhostSpriteR->GetPosition();
			if (maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 29] < 100 && maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 29] != 2) {
				_pGhostSpriteR->SetPosition(xpos, ypos);
				ghostSprites[rgcount] = _pGhostSpriteR;
				ghostLocs[rgcount] = ghostLoc;
				_pGame->AddSprite(ghostSprites[rgcount]);
				maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 29] = 203;
				ghostLoc = 0;
				rgcount++;
			}
		}

	}
	while (xygcount != numGhostXY)
	{
		if (ghostLoc == 0) {
			_pGhost1Sprite = new Sprite(_pGhost1Bitmap, rcBounds1, BA_BOUNCE);
			int xpos = ((rand() % 25) + 1) * 25;
			int ypos = ((rand() % 25) + 4) * 25;
			//_pGhost1Sprite->SetPosition(((rand() % 25) + 1) * 25, ((rand() % 25) + 4) * 25);
			//RECT  rcPos = _pGhost1Sprite->GetPosition();
			if (maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 1] < 100 && maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 1] != 2) {
				_pGhost1Sprite->SetPosition(xpos, ypos);
				ghostSpritesXY[xygcount] = _pGhost1Sprite;
				ghostLocsXY[xygcount] = ghostLoc;
				_pGame->AddSprite(ghostSpritesXY[xygcount]);
				maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 1] = 203;
				ghostLoc = 1;
				ghostSpritesXY[xygcount]->oldPos.x = -1;
				ghostSpritesXY[xygcount]->oldPos.y = -1;
				xygcount++;
			}
		}
		else if (ghostLoc == 1) {
			_pGhost1Sprite = new Sprite(_pGhost1Bitmap, rcBounds2, BA_BOUNCE);
			int xpos = ((rand() % 25) + 29) * 25;
			int ypos = ((rand() % 25) + 4) * 25;
			//_pGhost1Sprite->SetPosition(((rand() % 25) + 29) * 25, ((rand() % 25) + 4) * 25);
			//RECT  rcPos = _pGhost1Sprite->GetPosition();
			if (maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 29] < 100 && maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 29] != 2) {
				_pGhost1Sprite->SetPosition(xpos, ypos);
				ghostSpritesXY[xygcount] = _pGhost1Sprite;
				ghostLocsXY[xygcount] = ghostLoc;
				_pGame->AddSprite(ghostSpritesXY[xygcount]);
				maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 29] = 203;
				ghostLoc = 0;
				ghostSpritesXY[xygcount]->oldPos.x = -1;
				ghostSpritesXY[xygcount]->oldPos.y = -1;
				xygcount++;
			}
		}

	}
	_bGhostDead = FALSE;
}

void GhostMove(Sprite* ghostSprites[]) {

	for (int i = 0; i < numGhost; i++)
	{
		if (ghostSprites[i] == NULL) continue;
		RECT  rcPos = ghostSprites[i]->GetPosition();
		if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] == 203) {
			maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] = 1;//back track
		}
		if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 29] == 203) {
			maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 29] = 1;//back track
		}
		int _iRandom;
		int dirok = 0;
		int trynum = 0;
		while (true) {
			_iRandom = rand() % 100;
			trynum++;
			if (trynum > 10000) break;
			if (_iRandom < 50) {
				int dir2 = rand() % 4;
				ghostSprites[i]->dir = dir2;
			}
			if (ghostLocs[i] == 0) {
				if (ghostSprites[i]->dir == 0) {//left
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 2] < 100 &&
						maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 2] != 2) {
						rcPos.left = rcPos.left - 25;
						ghostSprites[i]->SetPosition(rcPos);
						dirok = 1;
					}
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 2] == 200) {
						_iNumLives--;
						//pacmanLoc = 0;
						//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						//RECT  newPos = _pPacManSprite->GetPosition();
						//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

						//rcPos.left = rcPos.left - 25;
						//ghostSprites[i]->SetPosition(rcPos);
						//maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] = 203;

						ghostSprites[i]->Kill();
						ghostSprites[i] = NULL;
						break;
					}
				}
				if (ghostSprites[i]->dir == 1) {//right
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25)] < 100 &&
						maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25)] != 2) {
						rcPos.left = rcPos.left + 25;
						ghostSprites[i]->SetPosition(rcPos);
						dirok = 1;
					}
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25)] == 200) {
						_iNumLives--;
						//pacmanLoc = 0;
						//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						//RECT  newPos = _pPacManSprite->GetPosition();
						//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

						//rcPos.left = rcPos.left + 25;
						//ghostSprites[i]->SetPosition(rcPos);
						//maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] = 203;
						ghostSprites[i]->Kill();
						ghostSprites[i] = NULL;
						break;
					}
				}
				if (ghostSprites[i]->dir == 2) {//top
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 1] < 100 &&
						maze3d[ghostLocs[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 1] != 2) {
						rcPos.top = rcPos.top - 25;
						ghostSprites[i]->SetPosition(rcPos);
						dirok = 1;
					}
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 1] == 200) {
						_iNumLives--;
						//pacmanLoc = 0;
						//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						//RECT  newPos = _pPacManSprite->GetPosition();
						//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

						//rcPos.top = rcPos.top - 25;
						//ghostSprites[i]->SetPosition(rcPos);
						//maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] = 203;

						ghostSprites[i]->Kill();
						ghostSprites[i] = NULL;
						break;
					}
				}
				if (ghostSprites[i]->dir == 3) {//bottom 
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 1] < 100 &&
						maze3d[ghostLocs[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 1] != 2) {
						rcPos.top = rcPos.top + 25;
						ghostSprites[i]->SetPosition(rcPos);
						dirok = 1;
					}
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 1] == 200) {
						_iNumLives--;
						//pacmanLoc = 0;
						//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						//RECT  newPos = _pPacManSprite->GetPosition();
						//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

						//rcPos.top = rcPos.top + 25;
						//ghostSprites[i]->SetPosition(rcPos);
						//maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] = 203;
						ghostSprites[i]->Kill();
						ghostSprites[i] = NULL;
						break;
					}
				}
			}
			else if (ghostLocs[i] == 1) {
				if (ghostSprites[i]->dir == 0) {//left
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 30] < 100 &&
						maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 30] != 2) {
						rcPos.left = rcPos.left - 25;
						ghostSprites[i]->SetPosition(rcPos);
						dirok = 1;
					}
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 30] == 200) {
						_iNumLives--;
						//pacmanLoc = 0;
						//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						//RECT  newPos = _pPacManSprite->GetPosition();
						//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

						//rcPos.left = rcPos.left - 25;
						//ghostSprites[i]->SetPosition(rcPos);
						//maze3d[ghostLocs[i]][(newPos.top / 25) - 4][(newPos.left / 25) - 29] = 203;
						ghostSprites[i]->Kill();
						ghostSprites[i] = NULL;
						break;
					}
				}
				if (ghostSprites[i]->dir == 1) {//right
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 28] < 100 &&
						maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 28] != 2) {
						rcPos.left = rcPos.left + 25;
						ghostSprites[i]->SetPosition(rcPos);
						dirok = 1;
					}
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 28] == 200) {
						_iNumLives--;
						//pacmanLoc = 0;
						//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						//RECT  newPos = _pPacManSprite->GetPosition();
						//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

						//rcPos.left = rcPos.left + 25;
						//ghostSprites[i]->SetPosition(rcPos);
						//maze3d[ghostLocs[i]][(newPos.top / 25) - 4][(newPos.left / 25) - 29] = 203;
						ghostSprites[i]->Kill();
						ghostSprites[i] = NULL;
						break;
					}
				}
				if (ghostSprites[i]->dir == 2) {//top
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 29] < 100 &&
						maze3d[ghostLocs[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 29] != 2) {
						rcPos.top = rcPos.top - 25;
						ghostSprites[i]->SetPosition(rcPos);
						dirok = 1;
					}
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 29] == 200) {
						_iNumLives--;
						//pacmanLoc = 0;
						//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						//RECT  newPos = _pPacManSprite->GetPosition();
						//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

						//rcPos.top = rcPos.top - 25;
						//ghostSprites[i]->SetPosition(rcPos);
						//maze3d[ghostLocs[i]][(newPos.top / 25) - 4][(newPos.left / 25) - 29] = 203;
						ghostSprites[i]->Kill();
						ghostSprites[i] = NULL;
						break;
					}
				}
				if (ghostSprites[i]->dir == 3) {//bottom
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 29] < 100 &&
						maze3d[ghostLocs[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 29] != 2) {
						rcPos.top = rcPos.top + 25;
						ghostSprites[i]->SetPosition(rcPos);
						dirok = 1;
					}
					if (maze3d[ghostLocs[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 29] == 200) {
						_iNumLives--;
						//pacmanLoc = 0;
						//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
						//RECT  newPos = _pPacManSprite->GetPosition();
						//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

						//rcPos.top = rcPos.top + 25;
						//ghostSprites[i]->SetPosition(rcPos);
						//maze3d[ghostLocs[i]][(newPos.top / 25) - 4][(newPos.left / 25) - 29] = 1;
						ghostSprites[i]->Kill();
						ghostSprites[i] = NULL;
						break;
					}
				}
			}

			if (dirok == 1) {
				if (ghostSprites[i] != NULL) {
					RECT  newPos = ghostSprites[i]->GetPosition();
					if (ghostLocs[i] == 0) {
						maze3d[ghostLocs[i]][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 203;
						break;
					}
					if (ghostLocs[i] == 1) {
						maze3d[ghostLocs[i]][(newPos.top / 25) - 4][(newPos.left / 25) - 29] = 203;
						break;
					}
				}

			}
		}
	}
}

void GhostMoveXY(Sprite* p1Sprite, Sprite* ghostSpritesXY[]) {

	for (int i = 0; i < numGhostXY; i++)
	{
		if (ghostSpritesXY[i] == NULL) continue;

		RECT  rcPos = ghostSpritesXY[i]->GetPosition();
		RECT rc1Pos = p1Sprite->GetPosition();

		if (ghostSpritesXY[i]->oldPos.x == rcPos.left && ghostSpritesXY[i]->oldPos.y == rcPos.top) {
			ghostSpritesXY[i]->dir++;
			if (ghostSpritesXY[i]->dir > 20) {
				while (true)
				{
					if (ghostLocsXY[i] == 0) {
						int xpos = ((rand() % 25) + 1) * 25;
						int ypos = ((rand() % 25) + 4) * 25;
						//_pGhost1Sprite->SetPosition(((rand() % 25) + 1) * 25, ((rand() % 25) + 4) * 25);
						//RECT  rcPos = _pGhost1Sprite->GetPosition();
						if (maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 1] < 100 && maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 1] != 2) {
							ghostSpritesXY[i]->SetPosition(xpos, ypos);
							maze3d[ghostLoc][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] = 1;  //sil
							maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 1] = 203;
							break;
						}
					}
					else if (ghostLocsXY[i] == 1) {
						int xpos = ((rand() % 25) + 29) * 25;
						int ypos = ((rand() % 25) + 4) * 25;
						//_pGhost1Sprite->SetPosition(((rand() % 25) + 29) * 25, ((rand() % 25) + 4) * 25);
						//RECT  rcPos = _pGhost1Sprite->GetPosition();
						if (maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 29] < 100 && maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 29] != 2) {
							ghostSpritesXY[i]->SetPosition(xpos, ypos);
							maze3d[ghostLoc][(rcPos.top / 25) - 4][(rcPos.left / 25) - 29] = 1;  //sil
							maze3d[ghostLoc][(ypos / 25) - 4][(xpos / 25) - 29] = 203;
							break;
						}
					}

				}
			}
		}
		else {
			ghostSpritesXY[i]->dir = 0;
		}
		ghostSpritesXY[i]->oldPos.x = rcPos.left;
		ghostSpritesXY[i]->oldPos.y = rcPos.top;



		if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] == 203) {
			maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] = 1;//back track
		}
		if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 29] == 203) {
			maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 29] = 1;//back track
		}

		int trynum = 0;
		while (true) {
			trynum++;
			if (trynum > 10000) break;

			if (ghostLocsXY[i] == 0) {
				if ((rand() % 3) == 0) {
					if (rcPos.left > rc1Pos.left) {
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 2] < 100 &&
							maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 2] != 2) {
							rcPos.left = rcPos.left - 25;
							ghostSpritesXY[i]->SetPosition(rcPos);

							if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 2] == 200) {
								_iNumLives--;
								//pacmanLoc = 0;
								//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
								//RECT  newPos = _pPacManSprite->GetPosition();
								//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

								ghostSpritesXY[i]->Kill();
								ghostSpritesXY[i] = NULL;
								break;
							}
						}
					}
					else if (rcPos.left < rc1Pos.left) {
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25)] < 100 &&
							maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25)] != 2) {
							rcPos.left = rcPos.left + 25;
							ghostSpritesXY[i]->SetPosition(rcPos);
						}

						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25)] == 200) {
							_iNumLives--;
							//pacmanLoc = 0;
							//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
							//RECT  newPos = _pPacManSprite->GetPosition();
							//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

							ghostSpritesXY[i]->Kill();
							ghostSpritesXY[i] = NULL;
							break;
						}
					}

					if (rcPos.top > rc1Pos.top) {
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 1] < 100 &&
							maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 1] != 2) {
							rcPos.top = rcPos.top - 25;
							ghostSpritesXY[i]->SetPosition(rcPos);
						}
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 1] == 200) {
							_iNumLives--;
							//pacmanLoc = 0;
							//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
							//RECT  newPos = _pPacManSprite->GetPosition();
							//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

							ghostSpritesXY[i]->Kill();
							ghostSpritesXY[i] = NULL;
							break;
						}
					}
					else if (rcPos.top < rc1Pos.top) {

						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 1] < 100 &&
							maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 1] != 2) {
							rcPos.top = rcPos.top + 25;
							ghostSpritesXY[i]->SetPosition(rcPos);
						}
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 1] == 200) {
							_iNumLives--;
							//pacmanLoc = 0;
							//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
							//RECT  newPos = _pPacManSprite->GetPosition();
							//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

							ghostSpritesXY[i]->Kill();
							ghostSpritesXY[i] = NULL;
							break;
						}
					}
				}
			}
			else if (ghostLocsXY[i] == 1) {
				if ((rand() % 3) == 0) {
					if (rcPos.top > rc1Pos.top) {
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 29] < 100 &&
							maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 29] != 2) {
							rcPos.top = rcPos.top - 25;
							ghostSpritesXY[i]->SetPosition(rcPos);
						}
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 29] == 200) {
							_iNumLives--;
							//pacmanLoc = 0;
							//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
							//RECT  newPos = _pPacManSprite->GetPosition();
							//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

							ghostSpritesXY[i]->Kill();
							ghostSpritesXY[i] = NULL;
							break;
						}
					}
					if (rcPos.top < rc1Pos.top) {
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 29] < 100 &&
							maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 29] != 2) {
							rcPos.top = rcPos.top + 25;
							ghostSpritesXY[i]->SetPosition(rcPos);
						}
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 29] == 200) {
							_iNumLives--;
							//pacmanLoc = 0;
							//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
							//RECT  newPos = _pPacManSprite->GetPosition();
							//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

							ghostSpritesXY[i]->Kill();
							ghostSpritesXY[i] = NULL;
							break;
						}
					}
					if (rcPos.left > rc1Pos.left) {
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 30] < 100 &&
							maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 30] != 2) {
							rcPos.left = rcPos.left - 25;
							ghostSpritesXY[i]->SetPosition(rcPos);
						}
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 30] == 200) {
							_iNumLives--;
							//pacmanLoc = 0;
							//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
							//RECT  newPos = _pPacManSprite->GetPosition();
							//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

							ghostSpritesXY[i]->Kill();
							ghostSpritesXY[i] = NULL;
							break;
						}
					}
					if (rcPos.left < rc1Pos.left) {

						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 28] < 100 &&
							maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 28] != 2) {
							rcPos.left = rcPos.left + 25;
							ghostSpritesXY[i]->SetPosition(rcPos);
						}
						if (maze3d[ghostLocsXY[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 28] == 200) {
							_iNumLives--;
							//pacmanLoc = 0;
							//_pPacManSprite->SetPosition(13 * 25, 15 * 25);
							//RECT  newPos = _pPacManSprite->GetPosition();
							//maze3d[pacmanLoc][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 200;//new pos

							ghostSpritesXY[i]->Kill();
							ghostSpritesXY[i] = NULL;
							break;
						}
					}
				}
			}
			if (ghostSprites[i] != NULL) {
				RECT  newPos = ghostSpritesXY[i]->GetPosition();
				if (ghostLocsXY[i] == 0) {
					maze3d[ghostLocsXY[i]][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 203;
					break;
				}
				if (ghostLocsXY[i] == 1) {
					maze3d[ghostLocsXY[i]][(newPos.top / 25) - 4][(newPos.left / 25) - 29] = 203;
					break;
				}

			}
			
		}
	}
}

void AddBoost() {
	RECT rcBounds1 = { 0, 0, 55 * 25, 30 * 25 };
	RECT rcBounds2 = { 0, 0, 55 * 25, 30 * 25 };

	int count = 0;
	int floor = 0;
	while (count < 2) {
		int xpos = ((rand() % 25) + 1) * 25;
		int ypos = ((rand() % 25) + 4) * 25;
		int x1pos = ((rand() % 25) + 29) * 25;
		int y1pos = ((rand() % 25) + 4) * 25;

		if (floor == 0) {
			_pCandySprite = new Sprite(_pCandyBitmap, rcBounds1, BA_BOUNCE);
			if (maze3d[floor][(ypos / 25) - 4][(xpos / 25) - 1] < 100 && maze3d[floor][(ypos / 25) - 4][(xpos / 25) - 1] != 2) {
				_pCandySprite->SetPosition(xpos, ypos);
				boostSprites[count] = _pCandySprite;
				_pGame->AddSprite(_pCandySprite);
				maze3d[floor][(ypos / 25) - 4][(xpos / 25) - 1] = 205;
				count++;
				floor = 1;
			}

		}
		else if (floor == 1) {
			_pCandySprite = new Sprite(_pCandyBitmap, rcBounds2, BA_BOUNCE);
			if (maze3d[floor][(y1pos / 25) - 4][(x1pos / 25) - 29] < 100 && maze3d[floor][(y1pos / 25) - 4][(x1pos / 25) - 29] != 2) {

				_pCandySprite->SetPosition(x1pos, y1pos);
				boostSprites[count] = _pCandySprite;
				_pGame->AddSprite(_pCandySprite);
				maze3d[floor][(y1pos / 25) - 4][(x1pos / 25) - 29] = 205;
				count++;
				floor = 0;
			}
		}
	}
}

void AddMine() {
	RECT rcBounds1 = { 0, 0, 55 * 25, 30 * 25 };
	RECT rcBounds2 = { 0, 0, 55 * 25, 30 * 25 };

	int count = 0;
	int floor = 0;
	while (count < 2) {

		int xpos = ((rand() % 25) + 1) * 25;
		int ypos = ((rand() % 25) + 4) * 25;
		int x1pos = ((rand() % 25) + 29) * 25;
		int y1pos = ((rand() % 25) + 4) * 25;

		if (floor == 0) {
			if (maze3d[floor][(ypos / 25) - 4][(xpos / 25) - 1] < 100 && maze3d[floor][(ypos / 25) - 4][(xpos / 25) - 1] != 2) {
				_pMineSprite = new Sprite(_pMineBitmap, rcBounds1, BA_BOUNCE);
				_pMineSprite->SetPosition(xpos, ypos);
				mineSprites[count] = _pMineSprite;
				mineLocs[count] = floor;
				_pGame->AddSprite(_pMineSprite);
				maze3d[floor][(ypos / 25) - 4][(xpos / 25) - 1] = 206;
				count++;
				floor = 1;
			}
		}
		if (floor == 1) {
			if (maze3d[floor][(y1pos / 25) - 4][(x1pos / 25) - 29] < 100 && maze3d[floor][(y1pos / 25) - 4][(x1pos / 25) - 29] != 2) {
				_pMineSprite = new Sprite(_pMineBitmap, rcBounds2, BA_BOUNCE);
				_pMineSprite->SetPosition(x1pos, y1pos);
				mineSprites[count] = _pMineSprite;
				mineLocs[count] = floor;
				_pGame->AddSprite(_pMineSprite);
				maze3d[floor][(y1pos / 25) - 4][(x1pos / 25) - 29] = 206;
				count++;
				floor = 0;
			}
		}
	}
}

void MineBoostMoveXY(Sprite* mineSprites[], Sprite* pPacSprite) {



	for (int i = 0; i < 2; i++)
	{

		RECT  rcPos = mineSprites[i]->GetPosition();
		RECT rc1Pos = pPacSprite->GetPosition();

		if (maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] == 206) {
			maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 1] = 1;//back track
		}
		if (maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 29] == 206) {
			maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 29] = 1;//back track
		}

		while (true) {

			if (mineLocs[i] == 0) {
				if ((rand() % 3) == 0) {
					if (rcPos.left > rc1Pos.left) {
						if (maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 2] < 100 &&
							maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 2] != 2) {
							rcPos.left = rcPos.left + 25;
							mineSprites[i]->SetPosition(rcPos);



						}
					}
					else if (rcPos.left < rc1Pos.left) {
						if (maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25)] < 100 &&
							maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25)] != 2) {
							rcPos.left = rcPos.left - 25;
							mineSprites[i]->SetPosition(rcPos);
						}



					}

					if (rcPos.top > rc1Pos.top) {
						if (maze3d[mineLocs[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 1] < 100 &&
							maze3d[mineLocs[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 1] != 2) {
							rcPos.top = rcPos.top + 25;
							mineSprites[i]->SetPosition(rcPos);
						}


					}
					else if (rcPos.top < rc1Pos.top) {
						if (maze3d[mineLocs[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 1] < 100 &&
							maze3d[mineLocs[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 1] != 2) {
							rcPos.top = rcPos.top - 25;
							mineSprites[i]->SetPosition(rcPos);
						}

					}


				}
			}
			else if (mineLocs[i] == 1) {
				if ((rand() % 3) == 0) {
					if (rcPos.left > rc1Pos.left) {
						if (maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 30] < 100 &&
							maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 30] != 2) {
							rcPos.left = rcPos.left + 25;
							mineSprites[i]->SetPosition(rcPos);
						}

					}
					if (rcPos.left < rc1Pos.left) {
						if (maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 28] < 100 &&
							maze3d[mineLocs[i]][(rcPos.top / 25) - 4][(rcPos.left / 25) - 28] != 2) {
							rcPos.left = rcPos.left - 25;
							mineSprites[i]->SetPosition(rcPos);
						}


					}

					if (rcPos.top > rc1Pos.top) {
						if (maze3d[mineLocs[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 29] < 100 &&
							maze3d[mineLocs[i]][(rcPos.top / 25) - 5][(rcPos.left / 25) - 29] != 2) {
							rcPos.top = rcPos.top + 25;
							mineSprites[i]->SetPosition(rcPos);
						}

					}
					if (rcPos.top < rc1Pos.top) {
						if (maze3d[mineLocs[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 29] < 100 &&
							maze3d[mineLocs[i]][(rcPos.top / 25) - 3][(rcPos.left / 25) - 29] != 2) {
							rcPos.top = rcPos.top - 25;
							mineSprites[i]->SetPosition(rcPos);
						}
					}



				}


				RECT  newPos = mineSprites[i]->GetPosition();
				if (mineLocs[i] == 0) {
					maze3d[mineLocs[i]][(newPos.top / 25) - 4][(newPos.left / 25) - 1] = 206;
					break;
				}
				if (mineLocs[i] == 1) {
					maze3d[mineLocs[i]][(newPos.top / 25) - 4][(newPos.left / 25) - 29] = 206;
					break;
				}



			}

		}
	}
}

