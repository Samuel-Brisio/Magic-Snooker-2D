//
// Created by samuelbrisio on 29/05/25.
//

#pragma once
#include "Actor.h"

enum class BallColor
{
    White,
    Blue,
    Red
};


class Ball : public Actor
{
public:
    explicit Ball(Game* game, float radius = 32.0f, float weight = 2.0f, BallColor color = BallColor::White);
    // void OnUpdate(float deltaTime) override;

private:
    float mRadius;
    float mWeight;
    BallColor mColor;
    bool mIsMoving;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawCircleComponent* mDrawComponent;
    class CircleColliderComponent* mColliderComponent;
};
