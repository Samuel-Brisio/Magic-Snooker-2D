//
// Created by samuelbrisio on 6/21/25.
//

#include "Cue.h"

Cue::Cue(Game *game, Ball *whiteBall, int width, int height)
    : Actor(game), mWhiteBall(whiteBall), mWidth(width), mHeight(height)
{
    // Distancia padrão que o taco deve aparecer longe da bola branca
    mDistance = 80.0f;
    mRotation = 0;
    mIncreaseRotation = Math::Pi;

    mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Cue/cue_resize.png",
       mWidth,  height, 2, Vector2::Zero, mRotation,  SDL_RendererFlip::SDL_FLIP_NONE);

    const auto ballPos = whiteBall->GetPosition();

    float deltaX = Math::Cos(mRotation) * mDistance;
    float deltaY = Math::Sin(mRotation) * mDistance;

    this->SetPosition(Vector2(ballPos.x+ deltaX, ballPos.y+ deltaY));

    SDL_Log("Cue::Cue : Cue Position %d %d", static_cast<int>(ballPos.x+ mDistance), static_cast<int>(ballPos.y+ mDistance));

    // mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Table/table_cut.png",
    // static_cast<int>(ballPos.x+ dist) + static_cast<int>(ballPos.y+ dist), 2);

    SDL_Log("Cue::Cue : Class Builder Done");
}

void Cue::OnUpdate(float deltaTime) {
    mRotation += mRotationDirection * mIncreaseRotation * deltaTime;

    mRotation = Math::Clamp(mRotation, -Math::Pi, Math::Pi);

    delete mDrawComponent;
    mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Cue/cue_resize.png",
   mWidth,  mHeight, 2, Vector2::Zero, mRotation, SDL_RendererFlip::SDL_FLIP_NONE);

    SDL_Log("Cue::Cue : Cue Rotation Direction %f; Cue Delta Rotation %f", mRotationDirection, mRotationDirection * mIncreaseRotation * deltaTime);

    // Se nenhum botão for pressionado, então não teremos um aumento na rotação do taco
    // if (mRotationDirection < 0.001 && mRotationDirection > -0.001) {
    //     // mRotation = 0;
    // }


    const auto ballPos = mWhiteBall->GetPosition();

    float deltaX = Math::Cos(mRotation) * mDistance;
    float deltaY = Math::Sin(mRotation) * mDistance;

    this->SetPosition(Vector2(ballPos.x+ deltaX, ballPos.y+ deltaY));

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
}
