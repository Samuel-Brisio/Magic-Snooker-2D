// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <SDL_image.h>
#include "CSV.h"
#include "Random.h"
#include "Game.h"

#include <thread>

#include "Actors/Actor.h"
#include "Actors/Mario.h"
#include "Actors/Block.h"
#include "Actors/Spawner.h"
#include "Actors/Table.h"
#include "Actors/Background.h"
#include "Actors/Ball.h"
#include "Actors/Cue.h"
#include "Actors/InvisibleAABBWall.h"
#include "Actors/WhiteBall.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "Components/ColliderComponents/AABBColliderComponent.h"
#include "UIElements/UIScreen.h"
#include "HUD.h"

Game::Game(int windowWidth, int windowHeight)
        :mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mLevelData(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mUpdatingActors(false)
        ,mWindowWidth(windowWidth)
        ,mWindowHeight(windowHeight)
        ,mHUD(nullptr)
        ,mScore(this)
{

}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Magic Snooker 2D", 50, 50, mWindowWidth, mWindowHeight, 0);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0)
    {
        SDL_Log("Failed to initialize SDL_ttf");
        return false;
    }

    // // Initialize SDL_mixer
    // if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    // {
    //     SDL_Log("Failed to initialize SDL_mixer");
    //     return false;
    // }

    Random::Init();

    mTicksCount = SDL_GetTicks();

    SetGamePlayState(GamePlayState::Playing);

    // Init all game actors
    InitializeActors();

    // Initially, change scene to MainMenu
    ChangeScene();

    return true;
}

void Game::InitializeActors()
{
    // Background
    auto bg = new Background(this, mWindowWidth, mWindowHeight);
    bg->SetPosition(Vector2(0.0, 0.0));

    // Initialize Table
    int table_width = mWindowWidth - 20;
    int table_height = table_width * 0.55;
    mTablePos = {10, 50, table_width, table_height};

    auto table = new Table(this, mTablePos);
    table->SetPosition(Vector2(mTablePos.x, mTablePos.y));

    new UIScreen(this, "../Assets/Fonts/SMB.ttf");

    // Initialize Balls setup
    int ballRadius = 16;
    Vector2 initialBallPosition = Vector2(200, 250);
    Vector2 currentBallPosition = initialBallPosition;

    for (int col = 1; col <= 5; col++) {
        for (int row = col; row <= 5; row++) {
            auto ball = new Ball(this, ballRadius, 0.5, (col + row)%2 ? BallColor::Blue : BallColor::Red);
            ball->SetPosition(currentBallPosition);
            currentBallPosition.y += 2*ballRadius + 1;
        }
        currentBallPosition = initialBallPosition;
        currentBallPosition.x += 2*col*ballRadius;
        currentBallPosition.y += col*ballRadius;
    }

    // Initialize White ball
    mWhiteBall = new WhiteBall(this, ballRadius, 0.5);
    mWhiteBall->SetPosition(Vector2(mTablePos.x + table_width - 200, mTablePos.y + table_height/2 - mWhiteBall->GetRadius() / 2));

    // // Test 01 -> Hit the opponent ball but also pocket its own ball
    // // Initialize White ball
    // mWhiteBall = new WhiteBall(this, ballRadius, 0.5);
    // mWhiteBall->SetPosition(Vector2(mTablePos.x + table_width/2, mTablePos.y + 180));
    // // ball->GetComponent<RigidBodyComponent>()->ApplyForce(Vector2(tablePos.x, tablePos.y) * 100);
    //
    // auto ball = new Ball(this, ballRadius, 0.5, BallColor::Blue);
    // ball->SetPosition(Vector2(mTablePos.x + table_width/2, mTablePos.y + 80));
    //
    // ball = new Ball(this, ballRadius, 0.5, BallColor::Red);
    // ball->SetPosition(Vector2(mTablePos.x + table_width/2, mTablePos.y + 120));

    // // Test 02 -> Pocket its own ball but also the white ball
    // // Initialize White ball
    // mWhiteBall = new WhiteBall(this, ballRadius, 0.5);
    // mWhiteBall->SetPosition(Vector2(mTablePos.x + table_width/2, mTablePos.y + 180));
    // // ball->GetComponent<RigidBodyComponent>()->ApplyForce(Vector2(tablePos.x, tablePos.y) * 100);
    //
    // auto ball = new Ball(this, ballRadius, 0.5, BallColor::Blue);
    // ball->SetPosition(Vector2(mTablePos.x + table_width/2, mTablePos.y + 70));
    //
    //

    // Initialize Cue
    mCue = new Cue(this, mWhiteBall, 300, 15);


}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                Quit();
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    for (auto actor : mActors)
    {
        actor->ProcessInput(state);
    }
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    // Update all actors and pending actors
    if (mGamePlayState != GamePlayState::Ending) {
        UpdateActors(deltaTime);
    }
    else {
        mEndingWaitTime -= deltaTime;
        if (mEndingWaitTime <= 0) {
            Shutdown();
            exit(0);
        }
    }

    // Reinsert UI screens
    for (auto ui : mUIStack) {
        if (ui->GetState() == UIScreen::UIState::Active) {
            ui->Update(deltaTime);
        }
    }

    // Delete any UIElements that are closed
    auto iter = mUIStack.begin();
    while (iter != mUIStack.end()) {
        if ((*iter)->GetState() == UIScreen::UIState::Closing) {
            delete *iter;
            iter = mUIStack.erase(iter);
        } else {
            ++iter;
        }
    }

    if (mHUD) {
        mHUD->SetPlayer1Score(mScore.GetPlayer1Score());
        mHUD->SetPlayer2Score(mScore.GetPlayer2Score());
        mHUD->SetCurrentPlayer(Score::GetCurrentPlayerStr(mScore.GetCurrentPlayer()));

    }
}

void Game::UpdateActors(float deltaTime)
{
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == ActorState::Destroy)
        {
            deadActors.emplace_back(actor);
        }
    }

    for (auto actor : deadActors)
    {
        delete actor;
        if (actor == mWhiteBall) {
            mWhiteBall = nullptr;
            mCue->RemoveWhiteBall();
            SDL_Log("White Ball is Destroyed");
            RespawnWhiteBall();
        }
    }

    bool haveBlueBall = std::any_of(mBalls.begin(), mBalls.end(), [](Ball* ball) {
    return ball->GetColor() == BallColor::Blue;
    });

    bool haveRedBall = std::any_of(mBalls.begin(), mBalls.end(), [](Ball* ball) {
    return ball->GetColor() == BallColor::Red;
});

    if (mWhiteBall != nullptr and (haveBlueBall and haveRedBall)) {
        bool allBallStopped = true;
        for (auto ball: mBalls) {
            if (ball->GetIsMoving()) allBallStopped = false;
        }

        // SDL_Log("Game::UpdateActors: allBallStopped=%d", allBallStopped);
        if (allBallStopped && mGamePlayState == GamePlayState::Simulating && mWhiteBall != nullptr) {
            mScore.EndTurn();
            TogglePlay();
            mCue->SetCueState(CueState::Moving);
        }
    }
    else {
        if (mScore.GetPlayer1Score() > mScore.GetPlayer2Score()) {
            mHUD->ShowEndGameScreen("Player 1 Wins!");
        }
        else if (mScore.GetPlayer2Score() > mScore.GetPlayer1Score()) {
            mHUD->ShowEndGameScreen("Player 2 Wins!");
        }
        else {
            mHUD->ShowEndGameScreen("It's a Tie!");
        }
        mGamePlayState = GamePlayState::Ending;
    }


}

void Game::AddActor(Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddDrawable(class DrawComponent *drawable)
{
    mDrawables.emplace_back(drawable);

    std::sort(mDrawables.begin(), mDrawables.end(),[](DrawComponent* a, DrawComponent* b) {
        return a->GetDrawOrder() < b->GetDrawOrder();
    });
}

void Game::RemoveDrawable(class DrawComponent *drawable)
{
    auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
    mDrawables.erase(iter);
}

void Game::AddAABBCollider(class AABBColliderComponent* collider)
{
    mAABBColliders.emplace_back(collider);
}

void Game::RemoveAABBCollider(AABBColliderComponent* collider)
{
    auto iter = std::find(mAABBColliders.begin(), mAABBColliders.end(), collider);
    mAABBColliders.erase(iter);
}
void Game::AddCircleCollider(class CircleColliderComponent* collider)
{
    mCircleColliders.emplace_back(collider);
}

void Game::RemoveCircleCollider(CircleColliderComponent* collider)
{
    auto iter = std::find(mCircleColliders.begin(), mCircleColliders.end(), collider);
    mCircleColliders.erase(iter);
}

void Game::AddOBBCollider(OBBColliderComponent* collider) {
    mOBBColliders.emplace_back(collider);
}

void Game::RemoveOBBCollider(OBBColliderComponent* collider) {
    auto iter = std::find(mOBBColliders.begin(), mOBBColliders.end(), collider);
    mOBBColliders.erase(iter);
}

void Game::AddBall(Ball* ball) {
    mBalls.emplace_back(ball);
}
void Game::RemoveBall(Ball* ball) {
    auto iter = std::find(mBalls.begin(), mBalls.end(), ball);
    mBalls.erase(iter);
}

void Game::AddBucket(Bucket* bucket) {
    mBuckets.emplace_back(bucket);
}
void Game::RemoveBucket(class Bucket* bucket) {
    auto iter = std::find(mBuckets.begin(), mBuckets.end(), bucket);
    mBuckets.erase(iter);
}
void Game::AddInvisibleOBBWall(class InvisibleOBBWall *obbWall) {
    mInvisibleOBBWalls.emplace_back(obbWall);
}
void Game::RemoveInvisibleOBBWall(class InvisibleOBBWall *obbWall) {
    auto iter = std::find(mInvisibleOBBWalls.begin(), mInvisibleOBBWalls.end(), obbWall);
    mInvisibleOBBWalls.erase(iter);
}
void Game::AddInvisibleAABBWall(class InvisibleAABBWall *aabbWall) {
    mInvisibleAABBWalls.emplace_back(aabbWall);
}
void Game::RemoveInvisibleAABBWall(class InvisibleAABBWall *aabbWall) {
    auto iter = std::find(mInvisibleAABBWalls.begin(), mInvisibleAABBWalls.end(), aabbWall);
    mInvisibleAABBWalls.erase(iter);
}






void Game::GenerateOutput()
{
    // Set draw color to black
    SDL_SetRenderDrawColor(mRenderer, 107, 140, 255, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    for (auto drawable : mDrawables)
    {
        if (drawable->IsVisible())
        {
            drawable->Draw(mRenderer);
        }
    }

    // Draw all UI screens
    for (auto ui :mUIStack)
    {
        ui->Draw(mRenderer);
    }


    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

SDL_Texture* Game::LoadTexture(const std::string& texturePath) {
    // --------------
    // TODO - PARTE 1
    // --------------

    // TODO 4.1 (~4 linhas): Utilize a função `IMG_Load` para carregar a imagem passada como parâmetro
    //  `texturePath`. Esse função retorna um ponteiro para `SDL_Surface*`. Retorne `nullptr` se a
    //  imagem não foi carregada com sucesso.
    SDL_Surface *surface = IMG_Load(texturePath.c_str());
    if (!surface) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
        return nullptr;
    }

    // TODO 4.2 (~6 linhas): Utilize a função `SDL_CreateTextureFromSurface` para criar uma textura a partir
    //  da imagem carregada anteriormente. Essa função retorna um ponteiro para `SDL_Texture*`. Logo após criar
    //  a textura, utilize a função `SDL_FreeSurface` para liberar a imagem carregada. Se a textura foi carregada
    //  com sucesso, retorne o ponteiro para a textura. Caso contrário, retorne `nullptr`.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return nullptr;
    }
    return texture;
}

std::string Game::printGamePlayState(Game::GamePlayState state) {
    switch (state) {
        case Game::GamePlayState::Playing:
            return "Playing";
        case Game::GamePlayState::Paused:
            return "Paused";
        case Game::GamePlayState::Simulating:
            return "Simulating";
        case Game::GamePlayState::Leaving:
            return "Leaving";
        case Game::GamePlayState::GameOver:
            return "GameOver";
        case Game::GamePlayState::LevelComplete:
            return "LevelComplete";
        default:
            return "Unknown";
    }
}

void Game::ToggleSimulation() {
    if (mGamePlayState == GamePlayState::Playing)
        SetGamePlayState(GamePlayState::Simulating);
    else {
        SDL_Log("Current Game State %s is not Playing, therefore Cannot toggle simulation", printGamePlayState(mGamePlayState).c_str());
        exit(0);
    }
}

void Game::TogglePlay() {
    if (mGamePlayState == GamePlayState::Playing) return;
    if (mGamePlayState == GamePlayState::Simulating) SetGamePlayState(GamePlayState::Playing);
    else {
        SDL_Log("Current Game State is not Simulating or Playing, therefore Cannot toggle Playing");
        exit(0);
    }
}


void Game::RespawnWhiteBall()
{
    int ballRadius = 16;
    Vector2 newPos;
    bool validPos = false;

    // Tente encontrar uma posição livre
    for (int attempts = 0; attempts < 100 && !validPos; ++attempts) {
        // Exemplo: escolha uma posição aleatória na mesa
        newPos = Vector2(mTablePos.x + 50 + rand() % 400, mTablePos.y + rand() % 200);
        validPos = true;
        for (auto ball : mBalls) {
            if ((ball->GetPosition() - newPos).Length() < 2 * ballRadius) {
                validPos = false;
                break;
            }
        }
    }

    // Crie e posicione a nova bola branca
    mWhiteBall = new WhiteBall(this, ballRadius, 0.5);
    mWhiteBall->SetPosition(newPos);
    mCue->SetWhiteBall(mWhiteBall);
}

bool Game::RemoveOneColorBall(BallColor color) {
    auto iter = std::find_if(mBalls.begin(), mBalls.end(), [color](Ball* ball) {
        return ball->GetColor() == color;
    });
    if (iter != mBalls.end()) {
        delete *iter; // Delete the ball
        SDL_Log("Removed one %s Ball", (color == BallColor::Red) ? "Red" : "Blue");
        return true;
    }
    return false; // No ball of that color found
}

void Game::UnloadScene()
{

    // Delete UI screens
    for (auto ui : mUIStack) {
        delete ui;
    }
    mUIStack.clear();

    // // Delete background texture
    // if (mBackgroundTexture) {
    //     SDL_DestroyTexture(mBackgroundTexture);
    //     mBackgroundTexture = nullptr;
    // }
}

void Game::ChangeScene()
{
    // Unload current Scene
    // UnloadScene();

    // // Reset game timer
    // mGameTimer = 0.0f;

    // Reset gameplay state
    // mGamePlayState = GamePlayState::Playing;

    mHUD = new HUD(this, "../Assets/Fonts/SMB.ttf");
    // mHUD->SetLevelName("Bla Foo");
}

SDL_Texture* Game::TextureFromSurface(SDL_Surface* surface) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

UIFont* Game::LoadFont(const std::string& fileName)
{
    // --------------
    // TODO - PARTE 1-1
    // --------------

    // TODO 1.: Verifique se o arquivo de fonte já está carregado no mapa mFonts.
    //  Se sim, retorne o ponteiro para a fonte carregada.
    //  Se não, crie um novo objeto UIFont, carregue a fonte do arquivo usando o método Load,
    //  e se o carregamento for bem-sucedido, adicione a fonte ao mapa mFonts.
    //  Se o carregamento falhar, descarregue a fonte com Unload e delete o objeto UIFont, retornando nullptr.
    if (mFonts.count(fileName) == 1) return mFonts[fileName];

    auto *font = new UIFont(mRenderer);
    bool success = font->Load(fileName);
    if (success) {
        mFonts.emplace(fileName, font);
        return font;
    }
    font->Unload();
    delete font;
    return nullptr;

}

void Game::Shutdown()
{
    // Delete actors
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    // // Delete level data
    // if (mLevelData != nullptr)
    // {
    //     for (int i = 0; i < LEVEL_HEIGHT; ++i)
    //     {
    //         if (mLevelData[i] != nullptr)
    //             delete[] mLevelData[i];
    //     }
    // }
    // delete[] mLevelData;

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
