//
// Created by samuelbrisio on 29/05/25.
//

#include "Table.h"


Table::Table(Game *game, const int width, const int height)
    : Actor(game), mWidth(width), mHeight(height)
{
    mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Table/table.png", mWidth, mHeight);
};