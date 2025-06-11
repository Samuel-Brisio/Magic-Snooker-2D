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
    auto colliders = mGame->GetColliders();

    for (auto collider : colliders) {
        bool isCollision = mColliderComponent->Intersect(collider);

        if (isCollision) mColliderComponent->SolveCollision(collider);
    }
}
