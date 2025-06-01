//
// Created by samuelbrisio on 6/1/25.
//

#pragma once
#include "Actor.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"

class Background : public Actor
{
public:
    explicit Background(Game* game, int width, int height);

private:
    int mWidth;
    int mHeight;
    DrawSpriteComponent* mDrawComponent;
};