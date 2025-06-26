//
// Created by samuelbrisio on 6/26/25.
//
#pragma once

#include "Actors/Ball.h"

class Score {
public:
    Score(BallColor color);

    void IncreaseScore() {mScore++;};
    void DecreaseScore() {mScore--;};
    void BallFellIntoPocket(BallColor ballColor);

    void ResetScore() {mScore = 0;};

private:
    int mScore;
    BallColor mBallColor;
};

