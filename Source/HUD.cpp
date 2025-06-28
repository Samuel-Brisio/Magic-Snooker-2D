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
    mPlayer2Score = AddText("00", Vector2(700, WORD_HEIGHT), Vector2(CHAR_WIDTH*2, WORD_HEIGHT), POINT_SIZE);


    // // TODO 4.: Adicione um texto com a string "1-1" logo abaixo do texto "World".
    mPlayerTurn = AddText("Player 1", Vector2(550 - CHAR_WIDTH*5 - WORD_OFFSET + CHAR_WIDTH, WORD_HEIGHT/2), Vector2(CHAR_WIDTH*8, WORD_HEIGHT), POINT_SIZE);


    // TODO 5.: Adicione um texto com a string "Mario" no canto superior esquerdo da tela, como no jogo original.
    AddText("Player 1 Score", Vector2(50, 0), Vector2(CHAR_WIDTH*14, WORD_HEIGHT), POINT_SIZE);

    // TODO 6.: Adicione um texto com a string "000" logo abaixo do texto "Player 1 Score".
    mPlayer1Score = AddText("00", Vector2(50, WORD_HEIGHT), Vector2(CHAR_WIDTH*2, WORD_HEIGHT), POINT_SIZE);

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