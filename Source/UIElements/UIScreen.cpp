// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "UIScreen.h"
#include "../Game.h"
#include "UIFont.h"

UIScreen::UIScreen(Game* game, const std::string& fontName)
	:mGame(game)
	,mPos(0.f, 0.f)
	,mSize(0.f, 0.f)
	,mState(UIState::Active)
    ,mSelectedButtonIndex(-1)
{
    // --------------
    // TODO - PARTE 1-1
    // --------------

    // TODO 1.: Adicione a UIScreen à lista de telas de UI do jogo (mGame) com PushUI(this).
    mGame->PushUI(this);

    // TODO 2.: Utilize o método LoadFont do jogo (mGame) para carregar a fonte padrão,
    //  passada como parâmetro (fontName).
    mFont =  mGame->LoadFont(fontName);
}

UIScreen::~UIScreen()
{
    // --------------
    // TODO - PARTE 1-1
    // --------------

    // TODO 1.: Percorra a listas de textos (mTexts) e delete cada UIText. Limpe a lista ao final do laço.
    for (const auto textUI: mTexts) {
        delete textUI;
    }
    mTexts.clear();

    // --------------
    // TODO - PARTE 1-2
    // --------------

    // TODO 1.: Percorra a lista de botões (mButtons) e delete cada UIButton. Limpe a lista ao final do laço.
    for (const auto buttonUI: mButtons ) {
        delete buttonUI;
    }
    mButtons.clear();
    // --------------
    // TODO - PARTE 1-3
    // --------------

    // TODO 1.: Percorra a lista de imagens (mImages) e delete cada UIImage. Limpe a lista ao final do laço.
    for (const auto imageUI: mImages ) {
        delete imageUI;
    }
    mImages.clear();
}

void UIScreen::Update(float deltaTime)
{
	
}

void UIScreen::Draw(SDL_Renderer *renderer)
{
    for (const auto imageUI: mImages ) {
        imageUI->Draw(renderer, mPos);
    }
    for (const auto textUI: mTexts) {
        textUI->Draw(renderer, mPos);
    }
    for (const auto buttonUI: mButtons ) {
        buttonUI->Draw(renderer, mPos);
    }
}

void UIScreen::ProcessInput(const uint8_t* keys)
{

}

void UIScreen::HandleKeyPress(int key)
{
    // --------------
    // TODO - PARTE 1-2
    // --------------

    // TODO 1.: Verifique se a tecla pressionada é W (SDLK_w), S (SDLK_s) ou Enter (SDLK_RETURN). Se a tecla for W,
    //  diminua o índice do botão selecionado (mSelectedButtonIndex) e destaque o botão anterior. Se o índice for
    //  menor que 0, defina-o como o último botão da lista. Se a tecla for S, aumente o índice do botão selecionado
    //  e destaque o próximo botão. Se o índice for maior ou igual ao tamanho da lista, defina-o como 0 (o primeiro botão).
    //  Se a tecla for Enter, verifique se o índice do botão selecionado é válido (maior ou igual a 0 e menor que
    //  o tamanho da lista). Se for, chame o método OnClick do botão selecionado.
    int numButtons = mButtons.size();

    // "-7 % 3 = -1" (Comportamento padrão do C++)
    // (a % b + b) % b; Garante que isso não aconteça
    if (key == SDLK_w) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex = ((mSelectedButtonIndex - 1) % numButtons + numButtons) % numButtons;
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);

    }
    else if (key == SDLK_s) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex = ((mSelectedButtonIndex + 1) % numButtons + numButtons) % numButtons;
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    }
    else if (key == SDLK_RETURN) {
        if (mSelectedButtonIndex < 0 || mSelectedButtonIndex >= numButtons) {
            SDL_Log("Invalid button index");
        }
        else {
            mButtons[mSelectedButtonIndex]->OnClick();
        }
    }
}

void UIScreen::Close()
{
	mState = UIState::Closing;
}

UIText* UIScreen::AddText(const std::string &name, const Vector2 &pos, const Vector2 &dims, const int pointSize, const int unsigned wrapLength)
{
    // --------------
    // TODO - PARTE 1-1
    // --------------

    // TODO 1.: Crie um novo UIText com o nome, fonte (mFont), tamanho do ponto (pointSize), comprimento de quebra (wrapLength),
    //  posição (pos), dimensões (dims) e cor branca. Armazene o ponteiro em uma variável t. A seguir, adicione o UIText
    //  à lista de textos (mTexts) e retorne o ponteiro t.
    const auto t = new UIText(name, mFont, pointSize, wrapLength, pos, dims, Vector3(1, 1, 1));
    mTexts.push_back(t);
    return t;

}

UIButton* UIScreen::AddButton(const std::string& name, const Vector2 &pos, const Vector2& dims, std::function<void()> onClick)
{
    // --------------
    // TODO - PARTE 1-2
    // --------------

    // TODO 1.: Crie um novo UIButton com o nome, fonte (mFont), função de clique (onClick),
    //  posição (pos), dimensões (dims) e cor laranja. Adicione o botão à lista de botões (mButtons).
    auto b = new UIButton(name, mFont, onClick, pos, dims, Vector3(200.0/255.0, 100.0/255.0, 0));
    mButtons.push_back(b);

    // TODO 2.: Se a lista de botões (mButtons) tiver apenas um botão, defina o índice do botão
    //  selecionado (mSelectedButtonIndex) como 0 e destaque o botão (b->SetHighlighted(true)).
    if (mButtons.size() == 1) {
        mSelectedButtonIndex = 0;
        b->SetHighlighted(true);
    }

    // TODO 3.: Retorne o ponteiro do botão criado (b).
    return b;
}

UIImage* UIScreen::AddImage(const std::string &imagePath, const Vector2 &pos, const Vector2 &dims, const Vector3 &color)
{
    // --------------
    // TODO - PARTE 1-3
    // --------------

    // TODO 1.: Crie um novo UIImage com o caminho da imagem (imagePath), posição (pos), dimensões (dims) e cor (color).
    //  Armazene o ponteiro em uma variável img. A seguir, adicione o UIImage à lista de imagens (mImages) e retorne
    //  o ponteiro img.
    const auto img = new UIImage(mGame, imagePath, pos, dims, color);
    // SDL_Log("Image Loaded: %p", img);
    mImages.push_back(img);
    return img;
}

void UIScreen::RemoveImage(UIImage *image) {
    auto p = std::find(mImages.begin(), mImages.end(), image);
    if (p != mImages.end()) {
        mImages.erase(p);
        delete image;
    }
}