//
// Created by gabri on 6/12/2025.
//

#include "InvisibleOBBWall.h"
#include "../Components/ColliderComponents/OBBColliderComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"

InvisibleOBBWall::InvisibleOBBWall(Game *game, Vector2 centerPosition, float width, float height, float rotation):
    Actor(game)
{
    game->AddInvisibleOBBWall(this);

    mOBBColliderComponent = new OBBColliderComponent(this, width, height);
    SetRotation(rotation);
    SetPosition(centerPosition);
    // std::vector<Vector2> corners = mOBBColliderComponent->GetCorners();

    // mDrawComponent = new DrawPolygonComponent(this, corners, 200);
}

InvisibleOBBWall::~InvisibleOBBWall() {
    mGame->RemoveInvisibleOBBWall(this);
}

