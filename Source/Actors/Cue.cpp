//
// Created by samuelbrisio on 6/21/25.
//

#include "Cue.h"

Cue::Cue(Game *game, Ball *whiteBall, int width, int height)
    : Actor(game)
    ,mWhiteBall(whiteBall)
    ,mWidth(width)
    ,mHeight(height)
    ,mCharge(false)
    ,mEnergy(0)
    ,mEnergyLevel(1)
    ,mState(CueState::Moving)
    ,mTimeDuration(0)
{
    // Distancia padrão que o taco deve aparecer longe da bola branca
    mDistance = 80.0f;
    mRotation = 0;
    mIncreaseRotation = Math::Pi;

    mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Cue/cue_resize.png",
       mWidth,  height, 2, Vector2::Zero, mRotation * (180 / Math::Pi),  SDL_RendererFlip::SDL_FLIP_NONE);

    // Define a posição do Taco em torno da bola branca
    const auto ballPos = whiteBall->GetPosition();

    float deltaX = Math::Cos(mRotation) * mDistance;
    float deltaY = Math::Sin(mRotation) * mDistance;

    this->SetPosition(Vector2(ballPos.x+ deltaX, ballPos.y+ deltaY));
}

void Cue::OnUpdate(float deltaTime) {
    // Change State
    if (mState == CueState::Moving && mCharge == true) mState = CueState::Charging;
    if (mState == CueState::Charging && mCharge == false) mState = CueState::Attacking;


    if (mState == CueState::Moving) {
        // Update the Cue Rotation
        mRotation += mRotationDirection * mIncreaseRotation * deltaTime;

        mRotation = Math::Clamp(mRotation, -Math::Pi, Math::Pi);


        // Create a new Sprit with the new Rotation
        delete mDrawComponent;
        mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Cue/cue_resize.png",
       mWidth,  mHeight, 2, Vector2::Zero, mRotation * (180 / Math::Pi), SDL_RendererFlip::SDL_FLIP_NONE);

        // Update Cue Position
        const auto ballPos = mWhiteBall->GetPosition();

        float deltaX = Math::Cos(mRotation) * mDistance * mEnergyLevel/3.0;
        float deltaY = Math::Sin(mRotation) * mDistance * mEnergyLevel/3.0;

        this->SetPosition(Vector2(ballPos.x+ deltaX, ballPos.y+ deltaY));

    }

    // Charge the energy meter
    if (mState == CueState::Charging) {
        mTimeDuration += deltaTime;
        mEnergyLevel = static_cast<int>(CalculateEnergyLevel(mTimeDuration));
        // SDL_Log("Energy Level: %i", mEnergyLevel);

        // Update Cue Position
        const auto ballPos = mWhiteBall->GetPosition();

        float deltaX = Math::Cos(mRotation) * mDistance * mEnergyLevel/3.0;
        float deltaY = Math::Sin(mRotation) * mDistance * mEnergyLevel/3.0;

        this->SetPosition(Vector2(ballPos.x+ deltaX, ballPos.y+ deltaY));
    }

    // The Cue hits the white ball
    if (mState == CueState::Attacking) {
        Vector2 direction = mWhiteBall->GetPosition() - this->GetPosition();
        float actualDistance = direction.Length();
        Vector2::Normalize(direction);

        if (actualDistance > mDistance) {
            // Update Cue Position
            SetPosition(this->GetPosition() + direction*0.1);
        }
        else {
            mWhiteBall->GetComponent<RigidBodyComponent>()->ApplyForce(direction * 10 * mEnergyLevel);
            SetPosition(Vector2::UnitX*1000);
            this->SetState(ActorState::Paused);
        }
    }

}

float Cue::CalculateEnergyLevel(double x) {
    mEnergy = (-4 * Math::Cos(x) + 2 * Math::Cos(2 * x) - (4.0/9.0) * Math::Cos(3 * x) + 0.25 * Math::Cos(4 * x) + 3) / 1;
    return std::ceil(mEnergy);
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

}
