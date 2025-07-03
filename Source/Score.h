//
// Created by samuelbrisio on 6/26/25.
//
#pragma once

#include <string>

#include "Actors/Ball.h"
#include <SDL.h>

class HUD;

enum class PlayerTurn {
    Player1,
    Player2,
};


class Score {
public:
    explicit Score(Game *game, BallColor player1BallColor = BallColor::Blue, BallColor player2BallColor = BallColor::Red);

    void IncreaseScore();
    void DecreaseScore();
    void BallFellIntoPocket(BallColor ballColor);

    void SetFirstHitBall(const Ball* ball) {
        SDL_Log("SetFirstHitBall: %s", ball->GetColor() == BallColor::Red ? "Red" : "Blue");
        mFirstHitBallInTurn = ball->GetColor();
    };

    void ResetScore() {mPlayer1Score = 0; mPlayer2Score = 0;};
    [[nodiscard]] PlayerTurn GetCurrentPlayer() const {return mPlayerTurn;};
    static std::string GetCurrentPlayerStr(PlayerTurn playerTurn);

    int GetPlayer1Score() const { return mPlayer1Score; }
    int GetPlayer2Score() const { return mPlayer2Score; }

    int GetPlayer1Energy() const { return mPlayer1Energy; }
    int GetPlayer2Energy() const { return mPlayer2Energy; }

    bool UsePower(int powerIndex);

    void EndTurn(HUD* hud);

private:
    Game* mGame;
    int mPlayer1Score;
    int mPlayer2Score;
    BallColor mPlayer1BallColor;
    BallColor mPlayer2BallColor;
    int mPlayer1Energy = 0;
    int mPlayer2Energy = 0;
    PlayerTurn mPlayerTurn;
    std::set<BallColor> mBallColors;
    BallColor mFirstHitBallInTurn;

    std::vector<bool> mPlayer1PowersUsed;
    std::vector<bool> mPlayer2PowersUsed;
};

