//
// Created by Lucas N. Ferreira on 08/12/23.
//

#pragma once

#include <string>
#include "UIElements/UIText.h"
#include "UIElements/UIScreen.h"

class Game;

class HUD : public UIScreen
{
public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 20.0f;
    const int WORD_OFFSET = 25.0f;
    const int CHAR_WIDTH = 20.0f;
    const int HUD_POS_Y = 10.0f;

    HUD(class Game* game, const std::string& fontName);
    ~HUD() ;

    // Reinsert the HUD elements
    void SetTime(int time);

    void SetCurrentPlayer(const std::string& levelName);
    void SetPlayer1Score(int score);
    void SetPlayer2Score(int score);
    void ShowEndGameScreen(const std::string &message);

    void SetPlayerEnergy(int player1Energy, int player2Energy);

    void ToggleEnablePowerSprite(int player, int idx);
    void ToggleDisablePowerSprite(int player, int idx);
    void EnableAllPowerSprites(int player);
    void DisableAllPowerSprites(int player);

private:
    // HUD elements
    UIText* mScoreCounter;
    UIText* mPlayerTurn;
    UIText* mLivesCount;
    UIText* mTimeText;
    UIText* mPlayer1Score;
    UIText* mPlayer2Score;

    UIImage* mPlayer1EnergyBar;
    UIImage* mPlayer2EnergyBar;

    std::vector<UIImage*> mPlayer1PowerSprite;
    std::vector<UIImage*> mPlayer2PowerSprite;

    std::vector<std::map<std::string, std::string>> mpowerSprites;
};
