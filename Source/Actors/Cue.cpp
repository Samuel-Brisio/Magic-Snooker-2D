//
// Created by samuelbrisio on 6/21/25.
//

#include "Cue.h"

#include "../Game.h"
#include "../Score.h"

Cue::Cue(Game *game, Ball *whiteBall, int width, int height)
    : Actor(game)
    ,mWhiteBall(whiteBall)
    ,mWidth(width)
    ,mHeight(height)
    ,mCharge(false)
    ,mEnergy(0)
    ,mEnergyLevel(1)
    ,mCueState(CueState::Moving)
    ,mTimeDuration(0)
    ,mIsCueAccelerationPowerUsed(false)
    ,mRotationDirection(0)
    ,mDelay(0.3f)
    ,mDirection(Vector2::Zero)
{
    // Distancia padrão que o taco deve aparecer longe da bola branca
    mDistance = 80.0f;
    mRotation = 0;
    mIncreaseRotation = Math::Pi/2;

    mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Cue/cue_resize.png",
       mWidth,  height, 1000, Vector2::Zero, mRotation * (180 / Math::Pi),  SDL_RendererFlip::SDL_FLIP_NONE);

    // Define a posição do Taco em torno da bola branca
    const auto ballPos = whiteBall->GetPosition();

    float deltaX = Math::Cos(mRotation) * mDistance;
    float deltaY = Math::Sin(mRotation) * mDistance;

    this->SetPosition(Vector2(ballPos.x+ deltaX, ballPos.y+ deltaY));

    mPowerUsed = -1; // No power used at the beginning
}

void Cue::OnUpdate(float deltaTime) {
    // Change State
    if (mCueState == CueState::Moving && mCharge == true) mCueState = CueState::Charging;
    if (mCueState == CueState::Charging && mCharge == false) mCueState = CueState::Attacking;


    if (mCueState == CueState::Moving && mGame->GetGamePlayState() == Game::GamePlayState::Playing) {
        // Update the Cue Rotation
        mRotation += mRotationDirection * mIncreaseRotation * deltaTime;

        if (mRotation < - 2*Math::Pi) mRotation = 0;
        if (mRotation >  2*Math::Pi) mRotation = 0;

        // Create a new Sprit with the new Rotation
        delete mDrawComponent;
        mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Cue/cue_resize_centralized.png",
        mWidth,  mHeight, 1000, Vector2::Zero, mRotation * (180 / Math::Pi), SDL_RendererFlip::SDL_FLIP_NONE);

        // Update Cue Position
        UpdateCuePosition();

        // Logic of Power Mechanic
        if (mPowerUsed != -1) {
            mGame->GetScore()->UsePower(mPowerUsed);
        }
        if (mGame->GetScore()->HasToApplyCueAccelerationPower()) {
            mIsCueAccelerationPowerUsed = true;
        }
    }

    // Charge the energy meter
    if (mCueState == CueState::Charging && mGame->GetGamePlayState() == Game::GamePlayState::Playing) {
        mTimeDuration += deltaTime;
        if (mIsCueAccelerationPowerUsed) {
            // Accelerate the time need to achieve max power in 8 times if the cue acceleration power is used
            mEnergyLevel = static_cast<int>(CalculateEnergyLevel(mTimeDuration * 8));
        }
        else {
            // Normal time to achieve max power
            mEnergyLevel = static_cast<int>(CalculateEnergyLevel(mTimeDuration));
        }

        // Update Cue Position
        UpdateCuePosition();

        mDirection = mWhiteBall->GetPosition() - this->GetCuePosition();
    }

    // The Cue hits the white ball
    if (mCueState == CueState::Attacking && mGame->GetGamePlayState() == Game::GamePlayState::Playing) {
        mIsCueAccelerationPowerUsed = false; // Reset the power used after applying it

        Vector2 direction_cpy = mWhiteBall->GetPosition() - this->GetCuePosition();
        float actualDistance = direction_cpy.Length();
        direction_cpy = Vector2::Normalize(direction_cpy);

        SDL_Log("Cue::OnUpdate: Direction Copy = (%.2f, %.2f)", direction_cpy.x, direction_cpy.y);

        if (actualDistance > mDistance) {
            // Update Cue Position
            SetPosition(this->GetPosition() + direction_cpy*16);
        }
        else {
            mDirection = Vector2::Normalize(mDirection);
            SDL_Log("Cue::OnUpdate: Direction = (%.2f, %.2f)", mDirection.x, mDirection.y);
            mWhiteBall->GetComponent<RigidBodyComponent>()->ApplyForce(mDirection * 400 * mEnergyLevel * mEnergyLevel);
            // SDL_Log("Cue::OnUpdate: Cue Hit the Ball");
            mCueState = CueState::Transition;
            mDelay = 0.3;

            mDirection = Vector2::Zero;
        }
    }

    if (mCueState == CueState::Transition) {
        if (mDelay > 0) mDelay -= deltaTime;
        else {
            SetPosition(Vector2::UnitX*2000);
            mGame->ToggleSimulation();
            mCueState = CueState::Waiting;
            mEnergyLevel = 1;
            mTimeDuration = 0;
            mEnergy = 0;
        }
    }

}

float Cue::CalculateEnergyLevel(double x) {
    mEnergy = (-4 * Math::Cos(x) + 2 * Math::Cos(2 * x) - (4.0/9.0) * Math::Cos(3 * x) + 0.25 * Math::Cos(4 * x) + 3) / 1;
    return std::ceil(mEnergy);
}

void Cue::UpdateCuePosition() {

    // Update Cue Position
    const auto ballPos = mWhiteBall->GetPosition();

    float deltaX = Math::Cos(mRotation) * mDistance * mEnergyLevel/3.0;
    float deltaY = Math::Sin(mRotation) * mDistance * mEnergyLevel/3.0;

    this->SetPosition(Vector2(ballPos.x+ deltaX, ballPos.y+ deltaY - mHeight / 2.0f)); // Adjust position to center the cue vertically
}


void Cue::OnProcessInput(const Uint8* keyState) {
    if (keyState[SDL_SCANCODE_LEFT]) {
        // SDL_Log("Cue::OnProcessInput : Left Key Pressed");
        mRotationDirection = 1.0;
    }
    else if (keyState[SDL_SCANCODE_RIGHT]) {
        mRotationDirection = -1.0;
    }
    else {
        mRotationDirection = 0.0;
    }

    if (keyState[SDL_SCANCODE_SPACE]) {
        mCharge = true;
    }
    else {
        mCharge = false;
    }

    if (keyState[SDL_SCANCODE_1]) {
        mPowerUsed = 1;
    }
    else if (keyState[SDL_SCANCODE_2]) {
        mPowerUsed = 2;
    }
    else {
        mPowerUsed = -1; // No power used
    }

}
