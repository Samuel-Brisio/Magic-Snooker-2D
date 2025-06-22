//
// Created by samuelbrisio on 6/21/25.
//

#pragma once
#include "Actor.h"
#include "Ball.h"
#include <cmath>
#include "../Components/DrawComponents/DrawAnimatedComponent.h"

enum class CueState
{
    Moving,
    Charging,
    Attacking,
};


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

    bool mCharge;
    float mEnergy;
    int mEnergyLevel;
    float mTimeDuration;

    CueState mState;

    class Ball* mWhiteBall;
    class DrawSpriteComponent* mDrawComponent;

    float CalculateEnergyLevel(double x);
};




