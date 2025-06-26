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
    Transition,
    Waiting
};


class Cue: public Actor {
public:
    explicit Cue(Game* game, class Ball* whiteBall, int width, int height);
    // ~Cue();
    void OnUpdate(float deltaTime) override;
    void OnProcessInput(const Uint8* keyState) override;
    void SetCueState(CueState state) {mCueState = state;};
    void SetWhiteBall(class Ball* whiteBall) {mWhiteBall = whiteBall;};
    class Ball* GetWhiteBall() {return mWhiteBall;};
    void RemoveWhiteBall() {mWhiteBall = nullptr;};

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
    float mDelay;

    CueState mCueState;

    class Ball* mWhiteBall;
    class DrawSpriteComponent* mDrawComponent;

    float CalculateEnergyLevel(double x);
};




