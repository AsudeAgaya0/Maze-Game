//-----------------------------------------------------------------
// Sprite Object
// C++ Header - AlienSprite.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Sprite.h"

//-----------------------------------------------------------------
// Sprite Class
//-----------------------------------------------------------------
class GhostSprite : public Sprite
{
public:
    // Constructor(s)/Destructor
    GhostSprite(Bitmap* pBitmap, RECT& rcBounds,
        BOUNDSACTION baBoundsAction = BA_STOP);
    virtual ~GhostSprite();

    // General Methods
    virtual SPRITEACTION  Update();
//
//    inline COORD CreateCoord(short x, short y)
//    {
//        COORD pos = { x, y };
//        return pos;
//    }
//
//     Helper function to see if 2 coords are equal (can't use == with coords).
//    inline bool CoordsEqual(const COORD& a, const COORD& b)
//    {
//        return a.X == b.X && a.Y == b.Y;
//    }
//
//#define MAZE_ROWS 31
//#define MAZE_COLS 55
//#define NUM_GHOSTS 4
//
//     Warp coordinates.
//#define WARP_LEFT_X 1
//#define WARP_RIGHT_X 53
//#define WARP_Y 14
//
//
//    GhostSprite(COORD startingpos);
//    void Move(char maze[MAZE_ROWS][MAZE_COLS], GhostSprite* ghostPtr, const COORD playerpos);
//    void Draw();
//    void SetCursorPosition(int left, int top);
//    void Kill(char maze[MAZE_ROWS][MAZE_COLS]);
//    void Reset(char maze[MAZE_ROWS][MAZE_COLS], COORD resetTo);
//    COORD GetPos() { return ghostPos; }
//    bool iscared; //Ghost is running away from player
//    bool incage;
//private:
//    bool AiSmartMoveCheck(char maze[MAZE_ROWS][MAZE_COLS], COORD move[], int& avoidmove, int directions);
//    COORD ghostPos;
//
//    int avoidMove;
//    int pastDirection; // Ghost heading direction


};
