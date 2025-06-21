//
// Created by Lucas N. Ferreira on 10/09/23.
//

#pragma once
#include "../Component.h"
#include "AABBColliderComponent.h"
#include "OBBColliderComponent.h"

class CircleColliderComponent : public Component
{
public:
    CircleColliderComponent(class Actor* owner, float radius, int updateOrder = 10);
    ~CircleColliderComponent() override;

    void SetRadius(float radius) { mRadius = radius; }
    float GetRadius() const;

    bool Intersect(const CircleColliderComponent* b) const;
    bool Intersect(const AABBColliderComponent* b) const;
    bool Intersect(const OBBColliderComponent* obb) const;

private:
    float mRadius;
};

