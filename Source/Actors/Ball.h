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
    explicit Ball(Game* game, float size = 1500.0f, float weight = 2.0f, BallColor color = BallColor::White);
    void OnUpdate(float deltaTime) override;

private:
    float mSize;
    float mWeight;
    BallColor mColor;
    bool mIsMoving;

    void ManageAnimations();

    // class Sound
    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
};
