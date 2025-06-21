//
// Created by gabri on 6/12/2025.
//

#ifndef OBBCOLLIDER_H
#define OBBCOLLIDER_H
#include <vector>
#include "../Component.h"
#include "../../Math.h"


class OBBColliderComponent: public Component {
public:
    OBBColliderComponent(class Actor* owner, float width, float height);
    ~OBBColliderComponent();

    std::vector<Vector2> GetCorners() const;

    Vector2 GetCenter() const;

    Vector2 GetExtents() const;

    Vector2 GetAxisX() const;

    Vector2 GetAxisY() const;

private:
    float mWidth;
    float mHeight;
};



#endif //OBBCOLLIDER_H
