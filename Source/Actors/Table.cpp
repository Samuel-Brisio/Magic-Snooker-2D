//
// Created by samuelbrisio on 29/05/25.
//

#include "Table.h"

#include "InvisibleCollider.h"

Table::Table(Game *game, const int width, const int height, int table_x_offset, int table_y_offset)
    : Actor(game), mWidth(width), mHeight(height)
{
    mDrawComponent = new DrawSpriteComponent(this, "../Assets/Sprites/Table/table_cut.png", mWidth, mHeight);


    // Bloco de Colisão da parte superior
    float top_pos = mHeight * 0.07 + table_y_offset;
    float bottom_pos = mHeight * 0.11 + table_y_offset;
    std::vector<Vector2> vertices_top;
    vertices_top.emplace_back(Vector2(10, top_pos));
    vertices_top.emplace_back(Vector2(10, bottom_pos));
    vertices_top.emplace_back(Vector2(10 + mWidth, bottom_pos));
    vertices_top.emplace_back(Vector2(10 + mWidth, top_pos));

    auto table_top_collider = new InvisibleCollider(mGame, vertices_top);

    //  Bloco de Colisão da parte inferior
    top_pos = mHeight - mHeight * 0.07 + table_y_offset;
    bottom_pos = mHeight - mHeight * 0.11 + table_y_offset;
    std::vector<Vector2> vertices_bottom;
    vertices_bottom.emplace_back(Vector2(10, top_pos));
    vertices_bottom.emplace_back(Vector2(10, bottom_pos));
    vertices_bottom.emplace_back(Vector2(10 + mWidth, bottom_pos));
    vertices_bottom.emplace_back(Vector2(10 + mWidth, top_pos));

    auto table_bottom_collider = new InvisibleCollider(mGame, vertices_bottom);

    //  Bloco de Colisão da parte esquerda
    auto left_pos = mWidth * 0.03 + table_x_offset;
    auto right_pos = mWidth * 0.055 + table_x_offset;
    std::vector<Vector2> vertices_left;
    vertices_left.emplace_back(Vector2(left_pos, table_y_offset));
    vertices_left.emplace_back(Vector2(left_pos, table_y_offset + mHeight));
    vertices_left.emplace_back(Vector2(right_pos, table_y_offset + mHeight));
    vertices_left.emplace_back(Vector2(right_pos, table_y_offset));

    auto table_left_collider = new InvisibleCollider(mGame, vertices_left);


    //  Bloco de Colisão da parte direita
    left_pos = mWidth - mWidth * 0.055  + table_x_offset;
    right_pos = mWidth - mWidth * 0.03 + table_x_offset;

    std::vector<Vector2> vertices_right;
    vertices_right.emplace_back(Vector2(left_pos, table_y_offset));
    vertices_right.emplace_back(Vector2(left_pos, table_y_offset + mHeight));
    vertices_right.emplace_back(Vector2(right_pos, table_y_offset + mHeight));
    vertices_right.emplace_back(Vector2(right_pos, table_y_offset));

    auto table_right_collider = new InvisibleCollider(mGame, vertices_right);

};