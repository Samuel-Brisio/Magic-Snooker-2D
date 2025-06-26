//
// Created by samuelbrisio on 29/05/25.
//

#include "WhiteBall.h"
#include "../Game.h"
#include "Bucket.h"
#include "InvisibleAABBWall.h"
#include "InvisibleOBBWall.h"
#include "../Components/ColliderComponents/CircleColliderComponent.h"
#include "../Components/DrawComponents/DrawCircleComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"


WhiteBall::WhiteBall(Game* game, float radius, float weight)
    : Ball(game, radius, weight, BallColor::White)
    ,mFirstHitBall(nullptr)
{

}

void WhiteBall::OnUpdate(float deltaTime) {
    auto aabbWalls = mGame->GetInvisibleAABBWalls();
    for (auto aabbWall : aabbWalls) {
        bool isCollision = mColliderComponent->Intersect(aabbWall->GetColliderComponent());
        if (isCollision) SolveCollision(aabbWall);
    }

    auto balls = mGame->GetBalls();
    for (auto ball : balls) {
        if (ball != this) {
            bool isCollision = mColliderComponent->Intersect(ball->GetColliderComponent());

            if (isCollision) {
                SolveCollision(ball);
            }
            if (mFirstHitBall !=nullptr) {
                mFirstHitBall = ball;
                mGame->GetScore()->SetFirstHitBall(mFirstHitBall);
            }
        }

    }

    auto obbWalls = mGame->GetInvisibleOBBWalls();
    for (auto obbWall : obbWalls) {
        bool isCollision = mColliderComponent->Intersect(obbWall->GetColliderComponent());
        if (isCollision) SolveCollision(obbWall);
    }

    auto buckets = mGame->GetBuckets();
    for (auto bucket : buckets) {
        bool isCollision = mColliderComponent->Intersect(bucket->GetColliderComponent());
        if (isCollision) SolveCollision(bucket);
    }

    // Check if the ball is moving
    if (mRigidBodyComponent->GetVelocity().Length() > 0.001) mIsMoving = true;
    else mIsMoving = false;

    // Reset the attribute mFirstHitBall
    if (mGame->GetGamePlayState() == Game::GamePlayState::Playing) {

        mFirstHitBall = nullptr;
    }


}