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
    mRigidBodyComponent = new RigidBodyComponent(this, weight);

    //@todo: remove initial force
    mRigidBodyComponent->ApplyForce(Vector2(160, 320));

    mColliderComponent = new CircleColliderComponent(this, mRadius);

    mDrawComponent = new DrawCircleComponent(this, 20, mRadius);
}

void Ball::OnUpdate(float deltaTime) {
    // verificar se houve colisão com a mesa
    auto coliders = mGame->GetColliders();

    for (auto collider : coliders) {
        bool isCollision = mColliderComponent->Intersect(collider);

        if (isCollision) SDL_Log("Collision detected");
    }
}
