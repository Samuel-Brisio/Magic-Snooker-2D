//
// Created by samuelbrisio on 29/05/25.
//

#pragma once
#include "Ball.h"



class WhiteBall : public Ball
{
public:
    explicit WhiteBall(Game* game, float radius = 32.0f, float weight = 2.0f);

    // void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;

private:
    Ball* mFirstHitBall;
};