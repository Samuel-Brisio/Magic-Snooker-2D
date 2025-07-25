//
// Created by samuelbrisio on 29/05/25.
//

#pragma once
#include "Actor.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"


class Table: public Actor {
public:
    explicit Table(Game* game, SDL_Rect position);
    // void OnUpdate(float deltaTime) override;
    SDL_Rect GetViableArea();

private:
    DrawSpriteComponent* mDrawComponent;
    SDL_Rect mTablePosDimensions;
    SDL_Rect mViableAreaPosDimensions;
    int mCollidersHeight;
};


