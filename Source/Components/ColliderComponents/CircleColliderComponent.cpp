//
// Created by Lucas N. Ferreira on 10/09/23.
//

#include "CircleColliderComponent.h"
#include "../../Actors/Actor.h"

CircleColliderComponent::CircleColliderComponent(class Actor* owner, const float radius, const int updateOrder)
        :Component(owner, updateOrder)
        ,mRadius(radius) {

}

const Vector2& CircleColliderComponent::GetCenter() const
{
    return mOwner->GetPosition();
}

float CircleColliderComponent::GetRadius() const
{
    return mOwner->GetScale() * mRadius;
}

bool CircleColliderComponent::Intersect(const CircleColliderComponent& c) const
{
    // Calculate amount squared
    Vector2 diff = GetCenter() - c.GetCenter();
    float distSq = diff.LengthSq();

    // Calculate sum of radii squared
    float radiiSq = GetRadius() + c.GetRadius();
    radiiSq *= radiiSq;

    return distSq <= radiiSq;
}

bool CircleColliderComponent::Intersect(const AABBColliderComponent* b) const {
    Vector2 min = b->GetMin();
    Vector2 max = b->GetMax();

    Vector2 center = GetCenter();
    float radius = mRadius;

    // Find the closest point on the AABB to the circle
    float closestX = std::max(min.x, std::min(center.x, max.x));
    float closestY = std::max(min.y, std::min(center.y, max.y));

    // Calculate the distance between the circle's center and this closest point
    float distanceX = center.x - closestX;
    float distanceY = center.y - closestY;

    // If the distance is less than the circle's radius, an intersection occurs
    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    return distanceSquared < (radius * radius);
}

void CircleColliderComponent::SolveCollision(const AABBColliderComponent* collider) {
    RigidBodyComponent* ownerRigidBody = mOwner->GetComponent<RigidBodyComponent>();

    Vector2 velocity = ownerRigidBody->GetVelocity();
    Vector2 center = this->GetCenter();
    float radius = this->GetRadius();

    Vector2 min = collider->GetMin();
    Vector2 max = collider->GetMax();

    // 1. Find the closest point on the AABB to the ball's center
    float closestX = std::max(min.x, std::min(center.x, max.x));
    float closestY = std::max(min.y, std::min(center.y, max.y));

    // 2. Calculate collision normal (direction from closest point to center)
    Vector2 collisionNormal = Vector2::Normalize(Vector2(center.x - closestX, center.y - closestY));


    // 3. Bounce the ball based on collision normal (reflect velocity)
    float dotProduct = velocity.x * collisionNormal.x + velocity.y * collisionNormal.y;
    Vector2 reflectedVelocity = velocity - 2.0f * dotProduct * collisionNormal;

    // 4. Apply restitution (optional: adds bounciness)
    float restitution = 0.8f; // Adjust for desired bounce effect
    reflectedVelocity *= restitution;

    // 5. Update velocity
    ownerRigidBody->SetVelocity(reflectedVelocity);

    // 6. Optional: Move the ball outside the collider to prevent sticking
    Vector2 penetration = collisionNormal * (radius - (center - Vector2(closestX, closestY)).Length());
    Vector2 newPosition = center + penetration;
    mOwner->SetPosition(newPosition);
}
