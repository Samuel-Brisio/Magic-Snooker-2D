// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "Math.h"
#include "Actors/Ball.h"
#include "Score.h"
#include "AudioSystem.h"

class HUD;

class Game
{
public:
    // static const int LEVEL_WIDTH = 215;
    // static const int LEVEL_HEIGHT = 15;
    // static const int TILE_SIZE = 32;
    // static const int SPAWN_DISTANCE = 700;
    static const int TRANSITION_TIME = 1;
    static const int NUM_POWERS = 2;
    static const int BALL_RADIUS = 16;

    enum class GameScene
    {
        MainMenu,
        Game,

    };

    enum class SceneManagerState
    {
        None,
        Entering,
        Active,
        Exiting
    };

    enum class GamePlayState
    {
        Playing,
        Simulating,
        Paused,
        GameOver,
        LevelComplete,
        Leaving,
        Ending,
    };

    Game(int windowWidth, int windowHeight);

    bool Initialize();
    void RunLoop();
    void Shutdown();
    void Quit() { mIsRunning = false; }

    // Actor functions
    void InitializeActors();
    void UpdateActors(float deltaTime);
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    // Draw functions
    void AddDrawable(class DrawComponent* drawable);
    void RemoveDrawable(class DrawComponent* drawable);

    // AABB Collider methods
    void AddAABBCollider(class AABBColliderComponent* collider);
    void RemoveAABBCollider(class AABBColliderComponent* collider);

    // Circle Collider methods
    void AddCircleCollider(class CircleColliderComponent* collider);
    void RemoveCircleCollider(class CircleColliderComponent* collider);
    std::vector<class AABBColliderComponent*>& GetAABBColliders() { return mAABBColliders; }
    std::vector<class CircleColliderComponent*>& GetCircleColliders() { return mCircleColliders; }
    std::vector<class OBBColliderComponent*>& GetOBBColliders() { return mOBBColliders; }

    // OBB Collider Methods
    void AddOBBCollider(class OBBColliderComponent* collider);
    void RemoveOBBCollider(class OBBColliderComponent* collider);

    // Actor Specific methods
    void AddBall(class Ball* ball);
    void RemoveBall(class Ball* ball);
    bool RemoveOneColorBall(BallColor color);
    std::vector<class Ball*> GetBalls() { return mBalls; }

    void AddInvisibleAABBWall(class InvisibleAABBWall * aabbWall);
    void RemoveInvisibleAABBWall(class InvisibleAABBWall * aabbWall);
    std::vector<class InvisibleAABBWall*> GetInvisibleAABBWalls(){ return mInvisibleAABBWalls; }

    void AddInvisibleOBBWall(class InvisibleOBBWall * obbWall);
    void RemoveInvisibleOBBWall(class InvisibleOBBWall * obbWall);
    std::vector<class InvisibleOBBWall*> GetInvisibleOBBWalls(){ return mInvisibleOBBWalls; }

    void AddBucket(class Bucket * bucket);
    void RemoveBucket(class Bucket * bucket);
    std::vector<class Bucket*> GetBuckets() { return mBuckets; }

    // Window functions
    int GetWindowWidth() const { return mWindowWidth; }
    int GetWindowHeight() const { return mWindowHeight; }

    int **GetLevelData() const { return mLevelData; }

    class AudioSystem* GetAudio() { return mAudio; }

    SDL_Texture* LoadTexture(const std::string& texturePath);

    void SetGamePlayState(GamePlayState state) {mGamePlayState = state;};
    void ToggleSimulation();
    void TogglePlay();
    GamePlayState GetGamePlayState() const { return mGamePlayState; }
    void RespawnWhiteBall();

    // Game-specific
    // const class Mario* GetMario() { return mMario; }
    std::string printGamePlayState(GamePlayState state);

    // Score-specific
    Score* GetScore() { return &mScore; }

    // UI functions
    SDL_Texture* TextureFromSurface(SDL_Surface* surface);
    void PushUI(class UIScreen* screen) { mUIStack.emplace_back(screen); }
    const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
    class UIFont* LoadFont(const std::string& fileName);

    class HUD *GetHUD() {return mHUD;};

    // scenes
    void SetGameScene(GameScene scene, float transitionTime = .0f);
    void ResetGameScene(float transitionTime = .0f);
    void UnloadScene();
    void LoadMainMenu();

private:
    void ProcessInput();
    void UpdateGame();
    // void UpdateCamera();
    void GenerateOutput();

    // Scene Manager
    void UpdateSceneManager(float deltaTime);
    void ChangeScene();
    SceneManagerState mSceneManagerState;
    float mSceneManagerTimer;

    // Track level state
    GameScene mGameScene;
    GameScene mNextScene;

    // Audio
    AudioSystem* mAudio;
    SoundHandle mMusicHandle;

    // Game-specific
    SDL_Rect mTablePos;

    // Load the level from a CSV file as a 2D array
    // int **LoadLevel(const std::string& fileName, int width, int height);
    // void BuildLevel(int** levelData, int width, int height);

    // All the actors in the game
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    // All the draw components
    std::vector<class DrawComponent*> mDrawables;

    // All the collision components
    std::vector<class AABBColliderComponent*> mAABBColliders;
    std::vector<class CircleColliderComponent*> mCircleColliders;
    std::vector<class OBBColliderComponent*> mOBBColliders;

    // Actors specific
    std::vector<class Ball*> mBalls;
    class WhiteBall* mWhiteBall;
    class Cue* mCue;
    std::vector<class InvisibleAABBWall*> mInvisibleAABBWalls;
    std::vector<class InvisibleOBBWall*> mInvisibleOBBWalls;
    std::vector<class Bucket*> mBuckets;
    class Table *mTable;

    // SDL stuff
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    // Window properties
    int mWindowWidth;
    int mWindowHeight;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mUpdatingActors;

    // Vector2 mCameraPos;

    // PlayerScore
    Score mScore;

    GamePlayState mGamePlayState;

    // Game-specific
    // class Mario *mMario;
    class HUD *mHUD;

    // All the UI elements
    std::vector<class UIScreen*> mUIStack;
    std::unordered_map<std::string, class UIFont*> mFonts;

    // Ending Waiting time
    float mEndingWaitTime = 5.0f;


    // Level data
    int **mLevelData;
};
