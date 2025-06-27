//
// Created by samuelbrisio on 6/26/25.
//

#include "Score.h"
#include <SDL.h>

Score::Score(const BallColor player1BallColor, const BallColor player2BallColor)
    : mPlayer1Score(0)
    ,mPlayer2Score(0)
    ,mPlayer1BallColor(player1BallColor)
    ,mPlayer2BallColor(player2BallColor)
    ,mPlayerTurn(PlayerTurn::Player1)
    ,mFirstHitBallInTurn(BallColor::None)
{
}


void Score::BallFellIntoPocket(BallColor ballColor) {
    mBallColors.insert(ballColor);
    if (ballColor == mPlayer1BallColor) {
        mPlayer1Score++;
    }
    else if (ballColor == mPlayer2BallColor) {
        mPlayer2Score++;
    }
}

void Score::IncreaseScore() {
    if (mPlayerTurn == PlayerTurn::Player1) {
        mPlayer1Score++;
    }
    else {
        mPlayer2Score++;
    }
}

void Score::DecreaseScore() {
    if (mPlayerTurn == PlayerTurn::Player1) {
        mPlayer1Score--;
    }
    else {
        mPlayer2Score--;
    }
}

void Score::EndTurn() {
    SDL_Log("End Turn: %s", GetCurrentPlayerStr(mPlayerTurn).c_str());

    if (mPlayerTurn == PlayerTurn::Player1) {
        if (mFirstHitBallInTurn == mPlayer2BallColor) {
            mPlayerTurn = PlayerTurn::Player2;
            // Delete a Player2 Ball
        }

        // Player Pocket White Ball
        else if (mBallColors.count(BallColor::White) != 0) {
            mPlayerTurn = PlayerTurn::Player2;
            // Do something
        }

        else if (mBallColors.count(mPlayer1BallColor) != 0) {
            mPlayerTurn = PlayerTurn::Player1;
        }

        else {
            mPlayerTurn = PlayerTurn::Player2;
        }
    }

    if (mPlayerTurn == PlayerTurn::Player2) {
        if (mFirstHitBallInTurn == mPlayer1BallColor) {
            mPlayerTurn = PlayerTurn::Player1;
            // Delete a Player2 Ball
        }

        // Player Pocket White Ball
        else if (mBallColors.count(BallColor::White) != 0) {
            mPlayerTurn = PlayerTurn::Player1;
            // Do something
        }

        else if (mBallColors.count(mPlayer2BallColor) != 0) {
            mPlayerTurn = PlayerTurn::Player2;
        }

        else {
            mPlayerTurn = PlayerTurn::Player1;
        }
    }

    SDL_Log("Next Player Turn: %s", GetCurrentPlayerStr(mPlayerTurn).c_str());

    mFirstHitBallInTurn = BallColor::None;
    mBallColors.clear();
};

std::string Score::GetCurrentPlayerStr(const PlayerTurn playerTurn) {
    if (playerTurn == PlayerTurn::Player1) {return "Player1";}
    if (playerTurn == PlayerTurn::Player2) {return "Player2";}
    return "Error: Unknown Player Turn";
}