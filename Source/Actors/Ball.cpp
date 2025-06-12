//
// Created by samuelbrisio on 29/05/25.
//

#include "Ball.h"

#include "../Game.h"
#include "../Components/ColliderComponents/CircleColliderComponent.h"
#include "../Components/DrawComponents/DrawCircleComponent.h"

Ball::Ball(Game *game, float radius, float weight, BallColor color)
    : Actor(game)
    ,mRadius(radius)
    ,mWeight(weight)
    ,mColor(color)
{
    mRigidBodyComponent = new RigidBodyComponent(this, weight, 0.1);

    mColliderComponent = new CircleColliderComponent(this, mRadius);

    mDrawComponent = new DrawCircleComponent(this, 20, mRadius, 100);
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
