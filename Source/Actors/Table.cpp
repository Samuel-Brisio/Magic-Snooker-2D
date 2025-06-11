//
// Created by samuelbrisio on 29/05/25.
//

#include "Table.h"

#include "InvisibleCollider.h"

Table::Table(Game *game, SDL_Rect position)
    : Actor(game)
{
    mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Table/table_cut.png",
        position.w,position.h, 2);
    
    // Top Collision Block
    SDL_Rect topRect = {position.x, position.y, position.w, int(position.h * 0.11)};
    new InvisibleCollider(mGame, topRect);
    //
    // // Bottom Collision Block
    SDL_Rect bottomRect = {position.x, position.y + position.h - int(position.h * 0.11), position.w, int(position.h * 0.11)};
    new InvisibleCollider(mGame, bottomRect);
    //
    // // Left Collision Block
    SDL_Rect leftRect = {position.x, position.y, int(position.h * 0.11), position.h};
    new InvisibleCollider(mGame, leftRect);

    // Right Collision Block
    SDL_Rect rightRect = {position.x + position.w - int(position.h * 0.11), position.y, int(position.h * 0.11), position.h};
    new InvisibleCollider(mGame, rightRect);

};