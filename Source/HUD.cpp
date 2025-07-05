//
// Created by Lucas N. Ferreira on 08/12/23.
//

#include "HUD.h"

HUD::HUD(class Game* game, const std::string& fontName)
    :UIScreen(game, fontName)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.: Adicione um texto com a string "Time" no canto superior direito da tela, como no jogo orginal. Note que
    //  a classe HUD tem constantes WORD_HEIGHT, WORD_OFFSET, CHAR_WIDTH, POINT_SIZE e HUD_POS_Y que podem ser usadas
    //  para posicionar e definir o tamanho do texto.
    AddText("Player 2 Score", Vector2(700, 0), Vector2(CHAR_WIDTH*14, WORD_HEIGHT), POINT_SIZE);

    // TODO 2.: Adicione um texto com a string "000" logo abaixo do texto "Player 2 Score".
    mPlayer2Score = AddText("00", Vector2(700, WORD_HEIGHT), Vector2(CHAR_WIDTH*1, WORD_HEIGHT), POINT_SIZE);


    // // TODO 4.: Adicione um texto com a string "1-1" logo abaixo do texto "World".
    mPlayerTurn = AddText("Player 1", Vector2(550 - CHAR_WIDTH*5 - WORD_OFFSET + CHAR_WIDTH, WORD_HEIGHT/2), Vector2(CHAR_WIDTH*8, WORD_HEIGHT), POINT_SIZE);


    // TODO 5.: Adicione um texto com a string "Mario" no canto superior esquerdo da tela, como no jogo original.
    AddText("Player 1 Score", Vector2(50, 0), Vector2(CHAR_WIDTH*14, WORD_HEIGHT), POINT_SIZE);

    // TODO 6.: Adicione um texto com a string "000" logo abaixo do texto "Player 1 Score".
    mPlayer1Score = AddText("00", Vector2(50, WORD_HEIGHT), Vector2(CHAR_WIDTH*1, WORD_HEIGHT), POINT_SIZE);


    // TODO 7.: Carrega as imagens de energia do jogador 1 e 2, e adiciona elas na HUD.
    SetPlayerEnergy(1, 0);

    // TODO 8.: Carrega o sprite dos poderes
    std::vector<std::map<std::string, std::string>> powerSprites = {
        {{"enable", "../Assets/Sprites/Magic/1.png"}, {"disable", "../Assets/Sprites/Magic/1_grayscale.png"}},
        {{"enable", "../Assets/Sprites/Magic/2.png"}, {"disable", "../Assets/Sprites/Magic/2_grayscale.png"}},
    };
    mpowerSprites = powerSprites;


    for (int i = 0; i < Game::NUM_POWERS; i++) {
        mPlayer1PowerSprite.push_back(AddImage(
        powerSprites[i]["enable"],
        Vector2(80 + (10+84*0.8)*(i) , mGame->GetWindowHeight() - 150),
        Vector2(84*0.8, 118*0.8)
        ));

        mPlayer2PowerSprite.push_back(AddImage(
         powerSprites[i]["disable"],
         Vector2(mGame->GetWindowWidth() - 260 + (10+84*0.8)*(i), mGame->GetWindowHeight() - 150),
         Vector2(84*0.8, 118*0.8)
        ));
    }
}

HUD::~HUD()
{

}

void HUD::SetTime(int time)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.: Utilize o método SetText() do mTimeText para atualizar o texto com o tempo restante. Lembre-se que
    //  o tempo é um inteiro que representa os segundos restantes, e deve ser convertido para string.
    std::string time_str = std::to_string(time);
    mTimeText->SetText(time_str);
    // snprintf(time_str, 4, "%03d", time);

    // TODO 2.: A posição e o tamanho do texto irão mudar dependendo do número de dígitos na variável time.
    //  Ajuste a posição e o tamanho do mTimeText de acordo com o número de dígitos, de tal forma que
    //  o texto fique alinhado à direita com o texto "Time" e o tamanho do texto seja proporcional ao número de dígitos.
    mTimeText->SetSize(Vector2(CHAR_WIDTH*time_str.size(), WORD_HEIGHT));
    // auto currentPos = mTimeText->GetPosition();
    // mTimeText->SetPosition(Vector2(currentPos.x + CHAR_WIDTH*(3 - time_str.size()), currentPos.y));

}

void HUD::SetPlayer1Score(int score) {
    std::string score_str = std::to_string(score);
    // snprintf(score_str, 3, "%02d", score);
    mPlayer1Score->SetText(score_str);
}

void HUD::SetPlayer2Score(int score) {
    std::string score_str = std::to_string(score);
    // snprintf(score_str, 3, "%02d", score);
    mPlayer2Score->SetText(score_str);
}


void HUD::SetCurrentPlayer(const std::string &playerName)
{
    mPlayerTurn->SetText(playerName);
}

void HUD::ShowEndGameScreen(const std::string& message) {
    int text_size = message.size();
    AddText(
        message.c_str(),
        Vector2(mGame->GetWindowWidth() / 2 - CHAR_WIDTH * (text_size / 2.0),mGame->GetWindowHeight()/2.0),
        Vector2(CHAR_WIDTH * text_size, WORD_HEIGHT),
        POINT_SIZE
        );
}

void HUD::SetPlayerEnergy(int player1Energy, int player2Energy) {
    // TODO 7.: Carrega as imagens de energia do jogador 1 e 2, e adiciona elas na HUD.

    if (mPlayer1EnergyBar != nullptr) RemoveImage(mPlayer1EnergyBar);
    if (mPlayer2EnergyBar != nullptr) RemoveImage(mPlayer2EnergyBar);

    std::string energyImagePath = "../Assets/Sprites/EnergyBar/Energy_" + std::to_string(player1Energy) + ".png";
    mPlayer1EnergyBar = AddImage(
        energyImagePath,
        Vector2(40, mGame->GetWindowHeight() - 50),
        Vector2(250, WORD_HEIGHT*1.5)
    );

    energyImagePath = "../Assets/Sprites/EnergyBar/Energy_" + std::to_string(player2Energy) + ".png";
    mPlayer2EnergyBar = AddImage(
        energyImagePath,
        Vector2(mGame->GetWindowWidth()-300, mGame->GetWindowHeight() - 50),
        Vector2(250, WORD_HEIGHT*1.5)
    );
}

void HUD::ToggleEnablePowerSprite(int player, int idx) {
    if (player == 1) {
        if (mPlayer1PowerSprite[idx] != nullptr) {
            RemoveImage(mPlayer1PowerSprite[idx]);
        }
        mPlayer1PowerSprite[idx] = AddImage(
            mpowerSprites[idx]["enable"],
            Vector2(80 + (10+84*0.8)*(idx), mGame->GetWindowHeight() - 150),
            Vector2(84*0.8, 118*0.8)
        );
    }
    else if (player == 2) {
        if (mPlayer2PowerSprite[idx] != nullptr) {
            RemoveImage(mPlayer2PowerSprite[idx]);
        }
        mPlayer2PowerSprite[idx] = AddImage(
            mpowerSprites[idx]["enable"],
            Vector2(mGame->GetWindowWidth()-260 + (10+84*0.8)*(idx), mGame->GetWindowHeight() - 150),
            Vector2(84*0.8, 118*0.8)
        );
    }
}

void HUD::ToggleDisablePowerSprite(int player, int idx) {
        if (player == 1) {
            if (mPlayer1PowerSprite[idx] != nullptr) {
                RemoveImage(mPlayer1PowerSprite[idx]);
            }
            mPlayer1PowerSprite[idx] = AddImage(
                mpowerSprites[idx]["disable"],
                Vector2(80 + (10+84*0.8)*(idx), mGame->GetWindowHeight() - 150),
                Vector2(84*0.8, 118*0.8)
            );
        }
        else if (player == 2) {
            if (mPlayer2PowerSprite[idx] != nullptr) {
                RemoveImage(mPlayer2PowerSprite[idx]);
            }
            mPlayer2PowerSprite[idx] = AddImage(
                mpowerSprites[idx]["disable"],
                Vector2(mGame->GetWindowWidth()-260 + (10+84*0.8)*(idx), mGame->GetWindowHeight() - 150),
                Vector2(84*0.8, 118*0.8)
            );
    }
}

void HUD::EnableAllPowerSprites(int player) {
    for (int i = 0; i < mpowerSprites.size() ; i++) {
        if (player == 1) {
            ToggleEnablePowerSprite(1, i);
        }
        else if (player == 2) {
            ToggleEnablePowerSprite(2, i);
        }
    }
}

void HUD::DisableAllPowerSprites(int player) {
    for (int i = 0; i < mpowerSprites.size() ; i++) {
        if (player == 1) {
            ToggleDisablePowerSprite(1, i);
        }
        else if (player == 2) {
            ToggleDisablePowerSprite(2, i);
        }
    }
}
