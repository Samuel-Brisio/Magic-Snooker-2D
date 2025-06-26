//
// Created by samuelbrisio on 6/26/25.
//

#include "Score.h"

Score::Score(BallColor color) {
    mScore = 0;
    mBallColor = color;
}


void Score::BallFellIntoPocket(BallColor ballColor) {
    if (ballColor == mBallColor) IncreaseScore();
}