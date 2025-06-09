//
// Created by samuelbrisio on 29/05/25.
//

#include "Ball.h"

#include "../Components/ColliderComponents/CircleColliderComponent.h"
#include "../Components/DrawComponents/DrawCircleComponent.h"

Ball::Ball(Game *game, float radius, float weight, BallColor color)
    : Actor(game)
    ,mRadius(radius)
    ,mWeight(weight)
    ,mColor(color)
{
    mRigidBodyComponent = new RigidBodyComponent(this, weight);

    mColliderComponent = new CircleColliderComponent(this, mRadius);

    mDrawComponent = new DrawCircleComponent(this, 20, mRadius);
}
