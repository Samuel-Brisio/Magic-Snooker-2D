//
// Created by samuelbrisio on 29/05/25.
//

#pragma once
#include "Actor.h"


class Table: public Actor {
public:
    explicit Table(Game* game);
    void OnUpdate(float deltaTime) override;

private:
    class DrawAnimatedComponent* mDrawComponent;
};


