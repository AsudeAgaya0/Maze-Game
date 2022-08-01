
#include "GhostSprite.h"


GhostSprite::GhostSprite(Bitmap* pBitmap, RECT& rcBounds,
    BOUNDSACTION baBoundsAction) : Sprite(pBitmap, rcBounds,
        baBoundsAction)
{
}

GhostSprite::~GhostSprite()
{
}

//-----------------------------------------------------------------
// AlienSprite General Methods
//-----------------------------------------------------------------
SPRITEACTION GhostSprite::Update()
{
    // Call the base sprite Update() method
    SPRITEACTION saSpriteAction;
    saSpriteAction = Sprite::Update();
    return saSpriteAction;
}
//
//
//#include <iostream>
//using namespace std;
//#include <cstring>
//#include <ctime>
//#include <conio.h>
//#include "GhostSprite.h"
//
//
//GhostSprite::GhostSprite(Bitmap* pBitmap, RECT& rcBounds,
//	BOUNDSACTION baBoundsAction) : Sprite(pBitmap, rcBounds,
//		baBoundsAction)
//{
//}
//
//GhostSprite::~GhostSprite()
//{
//}
//
//GhostSprite::GhostSprite(COORD startingpos)
//{
//	ghostPos = startingpos;
//	avoidMove = -1;
//	incage = true;
//	pastDirection = rand() % 4;
//	Draw();
//
//}
//
////-----------------------------------------------------------------
//// Sprite General Methods
////-----------------------------------------------------------------
//SPRITEACTION GhostSprite::Update()
//{
//	// Call the base sprite Update() method
//	SPRITEACTION saSpriteAction;
//	saSpriteAction = Sprite::Update();
//	return saSpriteAction;
//}
//
//// AI Al
//void GhostSprite::Move(char maze[MAZE_ROWS][MAZE_COLS], GhostSprite* ghostPtr, COORD playerpos)
//{
//	do
//	{
//		COORD evilmove[4] = { CreateCoord(-2, 0), CreateCoord(0, -1), CreateCoord(2, 0), CreateCoord(0, 1) };
//		enum evilmove { left, up, right, down };
//		COORD temp;
//
//
//		while (true)
//		{
//			size_t i = rand() % 4;
//			int t[8] = { 0, 0, 0, 0, 0, -1, 1, 2 }; // used to add 5/8 probability
//
//			while (i == avoidMove)	//keep randomizing a choice until it is NOT avoidMove.
//				i = rand() % 4;
//
//			do
//			{
//				do
//				{
//					i = 1;
//					i += t[rand() % 8];
//				} while (i == avoidMove);
//
//				temp = CreateCoord(ghostPos.X + evilmove[i].X, ghostPos.Y + evilmove[i].Y);
//
//				if (maze[temp.Y][temp.X] != 1)
//				{
//					avoidMove = down;
//					break;
//				}
//
//			} while (true);
//
//
//			// Rudimentary position tracking algorithm based on quadrants
//			//(Only applied this to two ghosts otherwise it gets impossible)
//			COORD secondTemp[4] = { CreateCoord(ghostPos.X + evilmove[left].X, ghostPos.Y + evilmove[left].Y),
//				CreateCoord(ghostPos.X + evilmove[up].X, ghostPos.Y + evilmove[up].Y), CreateCoord(ghostPos.X + evilmove[right].X, ghostPos.Y + evilmove[right].Y),
//				CreateCoord(ghostPos.X + evilmove[down].X, ghostPos.Y + evilmove[down].Y) };
//			do
//			{
//				if (ghostPos.X > playerpos.X)	/////////////////////////////////////////////////////////////PAC IS TO THE LEFT
//				{
//					if (ghostPos.Y > playerpos.Y)	// IF PAC IS TO THE TOP LEFT 
//					{
//
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))
//						{
//							temp = secondTemp[up]; avoidMove = down;  break;	// MOVE UP OR
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, left)) //MOVE LEFT OR
//						{
//							temp = secondTemp[left]; avoidMove = right; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down)) // MOVE DOWN ELSE
//						{
//							temp = secondTemp[down]; avoidMove = up; break;
//						}
//						else
//						{
//							temp = secondTemp[right]; avoidMove = left;  break;
//						}
//					}
//					else if (ghostPos.Y < playerpos.Y)//////PAC IS TO THE BOTTOM LEFT
//					{
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))
//						{
//							temp = secondTemp[down]; avoidMove = up; break;	// MOVE DOWN OR
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, left)) // MOVE LEFT OR
//						{
//							temp = secondTemp[left]; avoidMove = right; break;
//						}
//
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))
//						{
//							temp = secondTemp[up];  avoidMove = down; break;
//						}
//						else
//						{
//							temp = secondTemp[right]; avoidMove = left;  break;
//						}
//
//					}
//				}
//				else if (ghostPos.Y == playerpos.Y)//PAC IS IN OUR ROW TO THE LEFT
//				{
//					if (AiSmartMoveCheck(maze, secondTemp, avoidMove, left))	//MOVE LEFT
//					{
//						temp = secondTemp[left]; avoidMove = right; break;
//					}
//					if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))		//MOVE UP
//					{
//						temp = secondTemp[up];  avoidMove = down; break;
//					}
//					if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))		//MOVE DOWN
//					{
//						temp = secondTemp[down]; avoidMove = up; break;
//					}
//					else
//					{
//						temp = secondTemp[right]; avoidMove = left;  break;
//					}
//				}
//
//				if (ghostPos.X < playerpos.X)	////////////////////////////////////////////////////////PAC IS TO THE RIGHT
//				{
//					if (ghostPos.Y > playerpos.Y)	// IF PAC IS TO THE TOP RIGHT 
//					{
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))		//MOVE UP
//						{
//							temp = secondTemp[up]; avoidMove = down; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))	//MOVE RIGHT
//						{
//							temp = secondTemp[right]; avoidMove = left; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))	//MOVE DOWN
//						{
//							temp = secondTemp[down]; avoidMove = up; break;
//						}
//						else
//						{
//							temp = secondTemp[left]; avoidMove = right;  break;
//						}
//					}
//					else if (ghostPos.Y < playerpos.Y)//PAC IS TO THE BOTTOM RIGHT
//					{
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))	//MOVE DOWN
//						{
//							temp = secondTemp[down]; avoidMove = up; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))		//MOVE RIGHT
//						{
//							temp = secondTemp[right]; avoidMove = left; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))		//MOVE UP
//						{
//							temp = secondTemp[up];  avoidMove = down; break;
//						}
//						else
//						{
//							temp = secondTemp[left]; avoidMove = right;  break;
//						}
//					}
//					else if (ghostPos.Y == playerpos.Y)//PAC IS IN OUR ROW TO THE LEFT
//					{
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))	//MOVE RIGHT
//						{
//							temp = secondTemp[right]; avoidMove = left; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))		//MOVE UP
//						{
//							temp = secondTemp[up]; avoidMove = down; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))	//MOVE DOWN
//						{
//							temp = secondTemp[down]; avoidMove = up; break;
//						}
//						else
//						{
//							temp = secondTemp[left]; avoidMove = right;  break;
//						}
//					}
//				}
//				if (ghostPos.X == playerpos.X)	////////////////////////////////PAC SAME COLUMN AS US
//				{
//					if (ghostPos.Y > playerpos.Y)							// IF PAC IS TO THE TOP RIGHT 
//					{
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, up))		//MOVE UP
//						{
//							temp = secondTemp[up]; avoidMove = down; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))	//MOVE RIGHT
//						{
//							temp = secondTemp[right]; avoidMove = left; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, left)) {	//MOVE LEFT
//							temp = secondTemp[left]; avoidMove = right; break;
//						}
//						else
//						{
//							temp = secondTemp[down]; avoidMove = up;  break;
//						}
//					}
//					else if (ghostPos.Y < playerpos.Y)								//PAC IS BELOW US
//					{
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, down))	//MOVE DOWN
//						{
//							temp = secondTemp[down]; avoidMove = up; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, right))	//MOVE RIGHT
//						{
//							temp = secondTemp[right]; avoidMove = left; break;
//						}
//						if (AiSmartMoveCheck(maze, secondTemp, avoidMove, left))	//MOVE LEFT
//						{
//							temp = secondTemp[left]; avoidMove = right; break;
//						}
//						else
//						{
//							temp = secondTemp[up]; avoidMove = down;  break;
//						}
//					}
//				}
//			} while (true);
//
//			temp = CreateCoord(ghostPos.X + evilmove[i].X, ghostPos.Y + evilmove[i].Y);
//
//			if (CoordsEqual(temp, CreateCoord(WARP_RIGHT_X, WARP_Y)))       // comparing position with warp points.
//				temp = CreateCoord(WARP_LEFT_X, WARP_Y);
//			else if (CoordsEqual(temp, CreateCoord(WARP_LEFT_X, WARP_Y)))
//				temp = CreateCoord(WARP_RIGHT_X, WARP_Y);
//
//			if (maze[temp.Y][temp.X] != 1)
//			{
//				if (i == left)
//					avoidMove = right;
//				else if (i == up)
//					avoidMove = down;
//				else if (i == right)
//					avoidMove = left;
//				else if (i == down)
//					avoidMove = up;
//				break;
//			}
//			ghostPos = temp;		//assign temp to be permanent position of ghost.
//			Draw();
//			break;
//		}
//	} while (true);
//}
//
//
//bool GhostSprite::AiSmartMoveCheck(char maze[MAZE_ROWS][MAZE_COLS], COORD secondTemp[], int& avoidMove, int directions)
//{
//	// TEST IF MOVE direction IS POSSIBLE
//	if (maze[secondTemp[directions].Y][secondTemp[directions].X] != 1 && avoidMove != directions)
//		return true;
//	else
//		return false;
//}
//
//void GhostSprite::SetCursorPosition(int left, int top)
//{
//	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
//	CONSOLE_SCREEN_BUFFER_INFO info;
//	GetConsoleScreenBufferInfo(out, &info);
//
//	left = max(left, 0);
//	left = min(left, info.dwSize.X - 1);
//	top = max(top, 0);
//	top = min(top, info.dwSize.Y - 1);
//
//	COORD cp = { left, top, };
//	SetConsoleCursorPosition(out, cp);
//}
//
//void GhostSprite::Draw()
//{
//	SetCursorPosition(ghostPos.X, ghostPos.Y);
//
//}
//
//void GhostSprite::Kill(char maze[MAZE_ROWS][MAZE_COLS])
//{
//	ghostPos = CreateCoord(31, 14);
//	Draw();
//	avoidMove = -1;
//}
//
//// Reset Ghost to cage
//void GhostSprite::Reset(char maze[MAZE_ROWS][MAZE_COLS], COORD resetTo)
//{
//	avoidMove = -1;
//	ghostPos = resetTo;
//}

