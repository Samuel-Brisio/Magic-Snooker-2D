//
// Created by samuelbrisio on 29/05/25.
//

#pragma once
#include "Ball.h"


class ColorBall: Ball {
public:
    explicit ColorBall(Game* game, float size = 1500.0f, float weight = 3.0f, BallColor color = BallColor::Blue);
    void OnUpdate(float deltaTime) override;

private:
};


