//
// Created by samuelbrisio on 6/21/25.
//

#pragma once
#include "Actor.h"
#include "Ball.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"


class Cue: public Actor {
public:
    explicit Cue(Game* game, class Ball* whiteBall, int width, int height);
    // ~Cue();
    void OnUpdate(float deltaTime) override;
    void OnProcessInput(const Uint8* keyState) override;

private:
    float mForceLevel;
    float mDistance;
    float mIncreaseRotation;
    float mRotationDirection;
    int mWidth;
    int mHeight;

    class Ball* mWhiteBall;
    class DrawSpriteComponent* mDrawComponent;
};




