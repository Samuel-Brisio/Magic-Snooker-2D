//
// Created by samuelbrisio on 6/26/25.
//
#pragma once

#include <string>

#include "Actors/Ball.h"

enum class PlayerTurn {
    Player1,
    Player2,
};


class Score {
public:
    explicit Score(BallColor player1BallColor = BallColor::Blue, BallColor player2BallColor = BallColor::Red);

    void IncreaseScore();
    void DecreaseScore();
    void BallFellIntoPocket(BallColor ballColor);

    void SetFirstHitBall(const Ball* ball) {mFirstHitBallInTurn = ball->GetColor();};

    void ResetScore() {mPlayer1Score = 0; mPlayer2Score = 0;};
    [[nodiscard]] PlayerTurn GetCurrentPlayer() const {return mPlayerTurn;};
    static std::string GetCurrentPlayerStr(PlayerTurn playerTurn);

    int GetPlayer1Score() const { return mPlayer1Score; }
    int GetPlayer2Score() const { return mPlayer2Score; }

    void EndTurn();

private:
    int mPlayer1Score;
    int mPlayer2Score;
    BallColor mPlayer1BallColor;
    BallColor mPlayer2BallColor;
    PlayerTurn mPlayerTurn;
    std::set<BallColor> mBallColors;
    BallColor mFirstHitBallInTurn;
};

