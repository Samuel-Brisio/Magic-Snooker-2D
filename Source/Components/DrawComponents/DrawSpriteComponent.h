//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once
#include "DrawComponent.h"
#include <string>

class DrawSpriteComponent : public DrawComponent
{
public:
    // (Lower draw order corresponds with further back)
    DrawSpriteComponent(class Actor* owner, const std::string &texturePath,
        int width = 0, int height = 0, int drawOrder = 100, Vector2 positionOffset = Vector2::Zero, float rotation = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
    //
    // ~DrawSpriteComponent();

    void Draw(SDL_Renderer* renderer) override;

protected:
    // Map of textures loaded
    SDL_Texture* mSpriteSheetSurface;

    int mWidth;
    int mHeight;
    float mRotation;
    SDL_RendererFlip mRotationFlip;
    Vector2 mPositionOffset;
};