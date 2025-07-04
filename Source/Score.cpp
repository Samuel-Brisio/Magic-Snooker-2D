//
// Created by samuelbrisio on 6/26/25.
//

#include "Score.h"
#include <SDL.h>
#include "Game.h"
#include "HUD.h"

Score::Score(Game* game, const BallColor player1BallColor, const BallColor player2BallColor)
    :mGame(game)
    ,mPlayer1Score(0)
    ,mPlayer2Score(0)
    ,mPlayer1BallColor(player1BallColor)
    ,mPlayer2BallColor(player2BallColor)
    ,mPlayerTurn(PlayerTurn::Player1)
    ,mFirstHitBallInTurn(BallColor::None)
    ,mPlayer1Energy(1)
    ,mPlayer2Energy(0)
{
    for (int i = 0; i < Game::NUM_POWERS; i++) {
        mPlayer1PowersUsed.push_back(false);
        mPlayer2PowersUsed.push_back(false);
    }
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

void Score::EndTurn(HUD* hud) {
    PlayerTurn previousPlayerTurn = mPlayerTurn;
    SDL_Log("End Turn: %s", GetCurrentPlayerStr(mPlayerTurn).c_str());

    if (mPlayerTurn == PlayerTurn::Player1) {
        if (mFirstHitBallInTurn == mPlayer2BallColor) {
            SDL_Log("Player1 Hit Player2 Ball, Player2 Turn Started");
            mPlayerTurn = PlayerTurn::Player2;
            if (mGame->RemoveOneColorBall(mPlayer2BallColor)) mPlayer2Score++;
        }

        // Player Pocket White Ball
        else if (mBallColors.count(BallColor::White) != 0) {
            mPlayerTurn = PlayerTurn::Player2;
            if (mGame->RemoveOneColorBall(mPlayer2BallColor)) mPlayer2Score++;
        }

        else if (mBallColors.count(mPlayer1BallColor) != 0) {
            mPlayerTurn = PlayerTurn::Player1;
        }

        else {
            SDL_Log("Player1 Turn Ended without hit any ball, Player2 Turn Started");
            mPlayerTurn = PlayerTurn::Player2;
        }
    }

    else if (mPlayerTurn == PlayerTurn::Player2) {
        if (mFirstHitBallInTurn == mPlayer1BallColor) {
            mPlayerTurn = PlayerTurn::Player1;
            if (mGame->RemoveOneColorBall(mPlayer1BallColor)) mPlayer1Score++;
        }

        // Player Pocket White Ball
        else if (mBallColors.count(BallColor::White) != 0) {
            mPlayerTurn = PlayerTurn::Player1;
            if (mGame->RemoveOneColorBall(mPlayer1BallColor)) mPlayer1Score++;
        }

        else if (mBallColors.count(mPlayer2BallColor) != 0) {
            mPlayerTurn = PlayerTurn::Player2;
        }

        else {
            mPlayerTurn = PlayerTurn::Player1;
        }
    }


    // Power Logic

    // Player 1 Power Logic
    if (mPlayerTurn == PlayerTurn::Player1) {
        // Add One Energy to Player 1
        mPlayer1Energy = Math::Min(mPlayer1Energy + 1, 5);

        if (previousPlayerTurn == PlayerTurn::Player2) {
            // Reset Plater 1 Powers Used if the previous turn was Player 2
            for (int i = 0; i < Game::NUM_POWERS; i++) {
                mPlayer1PowersUsed[i] = false;
            }

            // Apply the powers used by Player 2 on Player 1
            for (int i = 0; i < Game::NUM_POWERS; i++) {
                if (mPlayer2PowersUsed[i] == true && i + 1 == 1) {
                    SDL_Log("Applying Player 2 Power 1 on Player 1");
                    mUseCueAccelerationPower = true;
                }
            }
        }

    }
    else {
        mPlayer2Energy = Math::Min(mPlayer2Energy + 1, 5);
        if (previousPlayerTurn == PlayerTurn::Player1) {
            // Reset Plater 2 Powers Used if the previous turn was Player 1
            for (int i = 0; i < Game::NUM_POWERS; i++) {
                mPlayer2PowersUsed[i] = false;
            }

            // Apply the powers used by Player 1 on Player 2
            for (int i = 0; i < Game::NUM_POWERS; i++) {
                if (mPlayer1PowersUsed[i] == true && i + 1 == 1) {
                    SDL_Log("Applying Player 1 Power 1 on Player 2");
                    mUseCueAccelerationPower = true;
                }
            }
        }
    }

    if (mPlayer1Energy > 0 && mPlayerTurn == PlayerTurn::Player1) mGame->GetHUD()->EnableAllPowerSprites(1);
    if (mPlayer2Energy > 0 && mPlayerTurn == PlayerTurn::Player2) mGame->GetHUD()->EnableAllPowerSprites(2);

    hud->SetPlayerEnergy(mPlayer1Energy, mPlayer2Energy);

    SDL_Log("Next Player Turn: %s", GetCurrentPlayerStr(mPlayerTurn).c_str());

    mFirstHitBallInTurn = BallColor::None;
    mBallColors.clear();
}

std::string Score::GetCurrentPlayerStr(const PlayerTurn playerTurn) {
    if (playerTurn == PlayerTurn::Player1) {return "Player 1";}
    if (playerTurn == PlayerTurn::Player2) {return "Player 2";}
    return "Error: Unknown Player Turn";
}

bool Score::UsePower(int powerIndex) {
    if (mPlayerTurn == PlayerTurn::Player1 && mPlayer1PowersUsed[powerIndex-1] == false) {
        if (mPlayer1Energy > 0) {
            mPlayer1Energy--;
            mPlayer1PowersUsed[powerIndex-1] = true;
            mGame->GetHUD()->SetPlayerEnergy(mPlayer1Energy, mPlayer2Energy);
            mGame->GetHUD()->ToggleDisablePowerSprite(1, powerIndex-1);
            SDL_Log("Player 1 used power %d, remaining energy: %d", powerIndex, mPlayer1Energy);

            if (mPlayer2Energy ==0) {
                mGame->GetHUD()->DisableAllPowerSprites(1);
            }
            return true;
        }
        mGame->GetHUD()->DisableAllPowerSprites(1);
        return false;
    }
    if (mPlayerTurn == PlayerTurn::Player2 && mPlayer2PowersUsed[powerIndex-1] == false) {
        if (mPlayer2Energy > 0) {
            mPlayer2Energy--;
            mPlayer2PowersUsed[powerIndex-1] = true;
            mGame->GetHUD()->SetPlayerEnergy(mPlayer1Energy, mPlayer2Energy);
            mGame->GetHUD()->ToggleDisablePowerSprite(2, powerIndex-1);
            SDL_Log("Player 2 used power %d, remaining energy: %d", powerIndex, mPlayer2Energy);

            if (mPlayer2Energy == 0) {
                mGame->GetHUD()->DisableAllPowerSprites(2);
            }
            return true;
        }
    }
    return false;
}

bool Score::HasToApplyCueAccelerationPower() {
    if (mUseCueAccelerationPower) {
        mUseCueAccelerationPower = false;
        return true;
    }
    return false;
}