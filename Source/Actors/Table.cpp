//
// Created by samuelbrisio on 29/05/25.
//

#include "Table.h"

#include "Bucket.h"
#include "InvisibleAABBWall.h"
#include "InvisibleOBBWall.h"

Table::Table(Game *game, SDL_Rect position)
    : Actor(game)
    , mTablePosDimensions(position)
{
    mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Table/table_cut.png",
        position.w,position.h, 2);

    int collidersHeight = int(position.h * 0.11);
    mCollidersHeight = collidersHeight;
    int horizontalWallsWidth = position.w/2 - 2*collidersHeight;
    int verticalWallsHeight = position.h - 3*collidersHeight;
    // Top Collision Block 1
    SDL_Rect topRect1 = {position.x + collidersHeight + 20, position.y, horizontalWallsWidth, collidersHeight};
    new InvisibleAABBWall(mGame, topRect1);
    mViableAreaPosDimensions.y = topRect1.y + topRect1.h + 5;

    // Top Collision Block 2
    SDL_Rect topRect2 = {position.x + position.w/2 + collidersHeight/2 + 10, position.y, horizontalWallsWidth, collidersHeight};
    new InvisibleAABBWall(mGame, topRect2);

    // Bottom Collision Block 1
    SDL_Rect bottomRect1 = {position.x + collidersHeight + 20, position.y + position.h - collidersHeight, horizontalWallsWidth, collidersHeight};
    new InvisibleAABBWall(mGame, bottomRect1);
    mViableAreaPosDimensions.h = (bottomRect1.y - 5) - mViableAreaPosDimensions.y;

    // Bottom Collision Block 2
    SDL_Rect bottomRect2 = {position.x + position.w/2 + collidersHeight/2 + 10, position.y + position.h - collidersHeight, horizontalWallsWidth, collidersHeight};
    new InvisibleAABBWall(mGame, bottomRect2);

    // Left Collision Block
    SDL_Rect leftRect = {position.x - 5, position.y+ collidersHeight + 28, collidersHeight, verticalWallsHeight-6};
    new InvisibleAABBWall(mGame, leftRect);
    mViableAreaPosDimensions.x = leftRect.x + leftRect.w + 5;

    // Right Collision Block
    SDL_Rect rightRect = {position.x + position.w - collidersHeight + 5, position.y+ collidersHeight + 28, collidersHeight-6, verticalWallsHeight};
    new InvisibleAABBWall(mGame, rightRect);
    mViableAreaPosDimensions.w = (rightRect.x - 5) - mViableAreaPosDimensions.x;

    Vector2 tableTopLeftReference = Vector2(position.x, position.y);
    // Top Left Bucket 1
    Vector2 topLeftBucketPosition1 = tableTopLeftReference + Vector2(collidersHeight, 10);
    new InvisibleOBBWall(game, topLeftBucketPosition1 , 110, 32, -2.3);

    // Top Left Bucket 2
    Vector2 topLeftBucketPosition2 = tableTopLeftReference + Vector2(0, collidersHeight);
    new InvisibleOBBWall(game, topLeftBucketPosition2 , 110, 32, -2.3);

    // Top Mid Bucket 1
    Vector2 topMidBucketPosition1 = tableTopLeftReference + Vector2(position.w/2 + collidersHeight/3, 5);
    new InvisibleOBBWall(game, topMidBucketPosition1 , 110, 32, -2.3);

    // Top Mid Bucket 2
    Vector2 topMidBucketPosition2 = tableTopLeftReference + Vector2(position.w/2 - collidersHeight/3, 5);
    new InvisibleOBBWall(game, topMidBucketPosition2 , 110, 32, 2.3);

    // Top Right Bucket 1
    Vector2 topRightBucketPosition1 = tableTopLeftReference + Vector2(position.w - collidersHeight, 10);
    new InvisibleOBBWall(game, topRightBucketPosition1 , 110, 32, 2.3);

    // Top Right Bucket 2
    Vector2 topRightBucketPosition2 = tableTopLeftReference + Vector2(position.w, collidersHeight);
    new InvisibleOBBWall(game, topRightBucketPosition2 , 110, 32, 2.3);

    // Bottom Left Bucket 1
    Vector2 bottomLeftBucketPosition1 = tableTopLeftReference + Vector2(0,position.h - collidersHeight);
    new InvisibleOBBWall(game, bottomLeftBucketPosition1 , 110, 32, 2.3);

    // Bottom Left Bucket 2
    Vector2 bottomLeftBucketPosition2 = tableTopLeftReference + Vector2(collidersHeight, position.h);
    new InvisibleOBBWall(game, bottomLeftBucketPosition2 , 110, 32, 2.3);

    // Bottom Mid Bucket 1
    Vector2 bottomMidBucketPosition1 = tableTopLeftReference + Vector2(position.w/2 + collidersHeight/3, position.h);
    new InvisibleOBBWall(game, bottomMidBucketPosition1 , 110, 32, 2.3);

    // Bottom Mid Bucket 2
    Vector2 bottomMidBucketPosition2 = tableTopLeftReference + Vector2(position.w/2 - collidersHeight/3, position.h);
    new InvisibleOBBWall(game, bottomMidBucketPosition2 , 110, 32, -2.3);

    // Bottom Right Bucket 1
    Vector2 bottomRightBucketPosition1 = tableTopLeftReference + Vector2(position.w - collidersHeight, position.h);
    new InvisibleOBBWall(game, bottomRightBucketPosition1 , 110, 32, -2.3);

    // Bottom Right Bucket 2
    Vector2 bottomRightBucketPosition2 = tableTopLeftReference + Vector2(position.w, position.h - collidersHeight);
    new InvisibleOBBWall(game, bottomRightBucketPosition2 , 110, 32, -2.3);


    // Bucket top left
    new Bucket(game, Vector2(position.x + collidersHeight/2 + 15, position.y + collidersHeight/2 + 15), 32);

    // Bucket top mid
    new Bucket(game, Vector2(position.x + position.w/2, position.y + collidersHeight/2), 32);

    // Bucket top right
    new Bucket(game, Vector2(position.x + position.w - (collidersHeight/2 + 15), position.y + collidersHeight/2 + 15), 32);


    // Bucket bottom left
    new Bucket(game, Vector2(position.x + collidersHeight/2 + 15, position.y + position.h - (collidersHeight/2 + 15)), 32);

    // Bucket bottom mid
    new Bucket(game, Vector2(position.x + position.w/2, position.y + position.h -(collidersHeight/2)), 32);

    // Bucket bottom right
    new Bucket(game, Vector2(position.x + position.w - (collidersHeight/2 + 15), position.y + position.h - (collidersHeight/2 + 15)), 32);
};

SDL_Rect Table::GetViableArea()
{
    // SDL_Log("Table::GetViableArea: Viable Area Position: %d, %d, %d, %d",
    //        mViableAreaPosDimensions.x, mViableAreaPosDimensions.y,
    //        mViableAreaPosDimensions.w, mViableAreaPosDimensions.h);

    return mViableAreaPosDimensions;
}