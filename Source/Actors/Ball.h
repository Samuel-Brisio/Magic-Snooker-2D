//
// Created by samuelbrisio on 29/05/25.
//

#pragma once

#include "Actor.h"
#include "../Components/ColliderComponents/CircleColliderComponent.h"

enum class BallColor
{
    White,
    Blue,
    Red,
    None,
};


class Ball : public Actor
{
public:
    explicit Ball(Game* game, float radius = 32.0f, float weight = 2.0f, BallColor color = BallColor::Blue);
    ~Ball();
    void OnUpdate(float deltaTime) override;

    float GetRadius() const;

    void SolveCollision(class Ball*);
    void SolveCollision(class InvisibleAABBWall*);
    void SolveCollision(class InvisibleOBBWall*);
    void SolveCollision(class Bucket*);

    CircleColliderComponent* GetColliderComponent() const {return mColliderComponent;}

    bool GetIsMoving() const {return mIsMoving;};
    BallColor GetColor() const {return mColor;}

protected:
    float mRadius;
    float mWeight;
    BallColor mColor;
    bool mIsMoving;

    class RigidBodyComponent* mRigidBodyComponent;
    // class DrawCircleComponent* mDrawCircleComponent;
    class DrawSpriteComponent* mDrawSpriteComponent;
    class CircleColliderComponent* mColliderComponent;
};
