//
// Created by Lucas N. Ferreira on 10/09/23.
//

#include "CircleColliderComponent.h"

#include "../../Game.h"
#include "../../Actors/Actor.h"

CircleColliderComponent::CircleColliderComponent(class Actor* owner, const float radius, const int updateOrder)
        :Component(owner, updateOrder)
        ,mRadius(radius) {
    mOwner->GetGame()->AddCircleCollider(this);
}

CircleColliderComponent::~CircleColliderComponent() {
    mOwner->GetGame()->RemoveCircleCollider(this);
}

const Vector2& CircleColliderComponent::GetCenter() const
{
    return mOwner->GetPosition();
}

float CircleColliderComponent::GetRadius() const
{
    return mOwner->GetScale() * mRadius;
}

bool CircleColliderComponent::Intersect(const CircleColliderComponent* c) const
{
    // Calculate amount squared
    Vector2 diff = GetCenter() - c->GetCenter();
    float distSq = diff.LengthSq();

    // Calculate sum of radii squared
    float radiiSq = GetRadius() + c->GetRadius();
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

void CircleColliderComponent::SolveCollision(const CircleColliderComponent* other) {
    SDL_Log("Ball-Ball Collision");

    // Get positions and radii
    Vector2 posA = GetCenter();
    Vector2 posB = other->GetCenter();
    float radiusA = GetRadius();
    float radiusB = other->GetRadius();

    // Get velocities
    Vector2 velA = mOwner->GetComponent<RigidBodyComponent>()->GetVelocity();
    Vector2 velB = other->mOwner->GetComponent<RigidBodyComponent>()->GetVelocity();

    // 1. Calculate collision normal (direction from B to A)
    Vector2 collisionNormal = Vector2::Normalize((posA - posB));

    // 2. Calculate relative velocity
    Vector2 relativeVelocity = velA - velB;

    // 3. Calculate velocity along collision normal
    float velocityAlongNormal = Vector2::Dot(relativeVelocity, collisionNormal);

    // 4. Don't resolve if separating
    if (velocityAlongNormal > 0) return;

    // 5. Simplified physics with equal properties
    const float restitution = 0.8f; // Equal bounciness for both balls
    const float massFactor = 0.5f;  // Equal mass distribution

    // 6. Calculate impulse
    float impulse = -(1 + restitution) * velocityAlongNormal * massFactor;

    // 7. Apply impulses (equal and opposite)
    Vector2 impulseVec = impulse * collisionNormal;

    mOwner->GetComponent<RigidBodyComponent>()->ApplyForce(impulseVec);
    other->mOwner->GetComponent<RigidBodyComponent>()->ApplyForce(-1 * impulseVec);

    // 8. Simple position correction to prevent sticking
    Vector2 correction = Vector2::Normalize(posA - posB) *
                        ((radiusA + radiusB) - (posA - posB).Length()) * 0.5f;

    mOwner->SetPosition(posA + correction);
    other->mOwner->SetPosition(posB - correction);
}
