//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "DrawSpriteComponent.h"

#include <SDL_image.h>

#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawSpriteComponent::DrawSpriteComponent(class Actor* owner, const std::string &texturePath, const int width,
    const int height, const int drawOrder,  Vector2 positionOffset, float rotation, SDL_RendererFlip flip)
        :DrawComponent(owner, drawOrder)
        ,mPositionOffset(positionOffset)
        ,mWidth(width)
        ,mHeight(height)
        ,mRotation(rotation)
        ,mRotationFlip(flip)
{
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 1.1 (1 linhas): Utilize a função LoadTexture da classe Game para criar uma textura a partir da
    //  imagem `texturePath` passada como parâmetro no construtor. Armazene o ponteiro retornada (SDLTexture*)
    //  na variável membro 'mSpriteSheetSurface'.
    mSpriteSheetSurface = mOwner->GetGame()->LoadTexture(texturePath);
}

DrawSpriteComponent::~DrawSpriteComponent() {
    // SDL_Log("DrawSpriteComponent deleted");
    SDL_DestroyTexture(mSpriteSheetSurface);
}


void DrawSpriteComponent::Draw(SDL_Renderer *renderer)
{
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 1.2 (~5 linhas): Utilize a função SDL_RenderCopyEx para desenhar a textura armazenada
    //  no atributo mSpriteSheetSurface. Você terá que criar um SDL_Rect para definir a região
    //  da tela onde será desenhado o sprite. Para que o objeto seja desenhado em relação a posição da câmera,
    //  subtraia a posição da câmera da posição do objeto. Além disso, você terá que criar uma flag do tipo
    //  SDL_RendererFlip para definir se o sprite será desenhado virado à direita ou à
    //  esquerda. A orientação do sprite (esquerda ou direita) depende da rotação do objeto dono do sprite.
    //  Se a rotação for zero, o sprite deve ser desenhado virado à direita. Se for igual a MAth::Pi, deve
    //  ser desenhado à esquerda.
    // const auto relative_pos =  mOwner->GetPosition() - mOwner->GetGame()->GetCameraPos();
    const auto relative_pos =  mOwner->GetPosition() + mPositionOffset;
    const SDL_Rect destRect = {static_cast<int>(relative_pos.x), static_cast<int>(relative_pos.y), mWidth, mHeight};
    SDL_RendererFlip flip = mOwner->GetRotation() == Math::Pi ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    if (SDL_FLIP_NONE == mRotationFlip) flip = SDL_FLIP_NONE;

    // Se tiver rotação o ponto de rotação será a extremidade esquerda
    SDL_Point rotationCenter = { 0, mHeight / 2 };  // rotaciona a partir do centro da extremidade esquerda
    SDL_RenderCopyEx(renderer, mSpriteSheetSurface, nullptr, &destRect, mRotation, &rotationCenter, flip );
}