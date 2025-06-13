//
// Created by samuelbrisio on 29/05/25.
//

#include "Table.h"

#include "InvisibleAABBWall.h"
#include "InvisibleOBBWall.h"

Table::Table(Game *game, SDL_Rect position)
    : Actor(game)
{
    mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Table/table_cut.png",
        position.w,position.h, 2);

    int collidersHeight = int(position.h * 0.11);
    
    // Top Collision Block
    SDL_Rect topRect = {position.x, position.y, position.w, collidersHeight};
    new InvisibleAABBWall(mGame, topRect);
    //
    // // Bottom Collision Block
    SDL_Rect bottomRect = {position.x, position.y + position.h - collidersHeight, position.w, collidersHeight};
    new InvisibleAABBWall(mGame, bottomRect);
    //
    // // Left Collision Block
    SDL_Rect leftRect = {position.x, position.y, collidersHeight, position.h};
    new InvisibleAABBWall(mGame, leftRect);

    // Right Collision Block
    SDL_Rect rightRect = {position.x + position.w - collidersHeight, position.y, collidersHeight, position.h};
    new InvisibleAABBWall(mGame, rightRect);

    // Top Left Corner
    Vector2 topLeftCornerPosition = Vector2(position.x, position.y) + Vector2(collidersHeight, 10);
    new InvisibleOBBWall(game, topLeftCornerPosition , 110, 32, -2.3);


    Vector2 testcorner = Vector2(position.x + collidersHeight + 150, position.y + position.h / 2);
    new InvisibleOBBWall(game, testcorner , 110, 32, -2.3);



};