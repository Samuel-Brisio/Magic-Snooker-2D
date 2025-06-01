//
// Created by samuelbrisio on 6/1/25.
//

#include "Background.h"

Background::Background(Game *game, const int width, const int height)
    : Actor(game), mWidth(width), mHeight(height)
{
    mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Background.png", mWidth, mHeight, 1);
};