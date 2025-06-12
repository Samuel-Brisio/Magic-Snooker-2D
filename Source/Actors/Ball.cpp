//
// Created by samuelbrisio on 29/05/25.
//

#include "Ball.h"

#include "../Game.h"
#include "../Components/ColliderComponents/CircleColliderComponent.h"
#include "../Components/DrawComponents/DrawCircleComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Ball::Ball(Game *game, float radius, float weight, BallColor color)
    : Actor(game)
    ,mRadius(radius)
    ,mWeight(weight)
    ,mColor(color)
{
    mRigidBodyComponent = new RigidBodyComponent(this, weight, 0.1);

    mColliderComponent = new CircleColliderComponent(this, mRadius);

    mDrawCircleComponent = new DrawCircleComponent(this, 20, mRadius, 100);

    Vector2 drawPositionOffset(-radius, -radius);

    if (color == BallColor::White) {
        mDrawSpriteComponent = new DrawSpriteComponent(this, "../Assets/Sprites/WhiteBall/WhiteBall.png",
        32, 32, 200, drawPositionOffset);
    }
    else if (color == BallColor::Red) {
        mDrawSpriteComponent = new DrawSpriteComponent(this, "../Assets/Sprites/WhiteBall/WhiteBall.png",
        32, 32, 200, drawPositionOffset);
    }
    else if (color == BallColor::Blue) {
        mDrawSpriteComponent = new DrawSpriteComponent(this, "../Assets/Sprites/WhiteBall/WhiteBall.png",
        32, 32, 200, drawPositionOffset);
    }
}
void Ball::OnUpdate(float deltaTime) {
    auto aabbColiders = mGame->GetAABBColliders();

    for (auto collider : aabbColiders) {
        bool isCollision = mColliderComponent->Intersect(collider);

        if (isCollision) mColliderComponent->SolveCollision(collider);
    }

    auto circleColiders = mGame->GetCircleColliders();

    for (auto collider : circleColiders) {
        if (collider != mColliderComponent) {
            bool isCollision = mColliderComponent->Intersect(collider);

            if (isCollision) mColliderComponent->SolveCollision(collider);
        }
    }
}