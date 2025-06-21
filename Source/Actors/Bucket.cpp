//
// Created by gabri on 6/14/2025.
//

#include "Bucket.h"

#include "../Game.h"
#include "../Components/ColliderComponents/CircleColliderComponent.h"
#include "../Components/DrawComponents/DrawCircleComponent.h"

Bucket::Bucket(Game *game, Vector2 position, int radius):
    Actor(game)
{
    game->AddBucket(this);

    SetPosition(position);
    mColliderComponent = new CircleColliderComponent(this, radius);
    mDrawCircleComponent = new DrawCircleComponent(this, 20, radius, 200);
}

Bucket::~Bucket() {
    mGame->RemoveBucket(this);
}
