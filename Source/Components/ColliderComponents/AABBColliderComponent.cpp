//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AABBColliderComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

AABBColliderComponent::AABBColliderComponent(class Actor* owner, int dx, int dy, int w, int h,
        ColliderLayer layer, bool isStatic, int updateOrder)
        :Component(owner, updateOrder)
        ,mOffset(Vector2((float)dx, (float)dy))
        ,mIsStatic(isStatic)
        ,mWidth(w)
        ,mHeight(h)
        ,mLayer(layer)
{
    mOwner->GetGame()->AddAABBCollider(this);
}

AABBColliderComponent::~AABBColliderComponent()
{
    mOwner->GetGame()->RemoveAABBCollider(this);
}

Vector2 AABBColliderComponent::GetMin() const
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.1 (1 linha): Calcule (e retorne) o ponto mínimo dessa AABB. A variável `mOffset`
    //  define a posição da AABB com relação a posição do objeto dono do componente. Portanto,
    //  basta somar a posição do objeto dono do componente a esse deslocamento.
    return mOwner->GetPosition() + mOffset;
}

Vector2 AABBColliderComponent::GetMax() const
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.2 (1 linha): Calcule (e retorne) o ponto máximo dessa AABB. As variáveis membro
    //  `mWidth` e `mHeight` definem a altura e a largura da AABB, respectivamente. Portanto,
    //  basta somar a largura à coordenada x e a altura à coordenada y do ponto mínimo da AABB
    //  (utilize o método `GetMin` implementado na etapa anterior).
    auto min = GetMin();
    return Vector2(min.x + mWidth, min.y + mHeight);

}

bool AABBColliderComponent::Intersect(const AABBColliderComponent& b) const
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 2 (~5 linhas): Verifique se esta AABB está colidindo com a AABB b passada como parâmetro.
    //  Retorne verdadeiro se estiver e falso caso contrário. Utilize os métodos `GetMin` e `GetMax`
    //  para acessar os pontos de mínimo e máximo das duas AABBs. É importante destacar que quando os valores
    //  foram iguais, as AABBs NÃO estão colidindo. Portanto, utilize o operador < para verificar
    //  se as AABBs estão colidindo.
    auto a_above_b = this->GetMin().y <= b.GetMax().y;
    auto b_above_a = b.GetMin().y <= this->GetMax().y;
    auto a_left_b = this->GetMax().x <= b.GetMin().x;
    auto b_left_a = b.GetMin().x <= this->GetMax().x;
    if (a_above_b || b_above_a || a_left_b || b_left_a) {
        return false;
    }
    SDL_Log("a_above_b: %b | b_above_a: %b | a_left_b: %b | b_left_a: %b", a_above_b, b_above_a, a_left_b, b_left_a);
    return true;

}

float AABBColliderComponent::GetMinVerticalOverlap(AABBColliderComponent* b) const
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 3.1 (2 linhas): Calcule as interseções verticais (top e down) entre as duas AABBs.
    // A em cima de B
    auto AB = this->GetMin().y - b->GetMax().y;
    auto BA = this->GetMax().y - b->GetMin().y;

    // TODO 3.2 (1 linha): Encontre e retorne a interseção com menor valor absoluto.
    return Math::Min(AB, BA);
}

float AABBColliderComponent::GetMinHorizontalOverlap(AABBColliderComponent* b) const
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 3.3 (2 linhas): Calcule as interseções horizontais (right e left) entre as duas AABBs.
    auto AB = GetMax().x - b->GetMin().x;
    auto BA = this->GetMin().x - b->GetMax().x;

    // TODO 3.4 (1 linha): Encontre e retorne a interseção com menor valor absoluto.
    return Math::Min(AB, BA);
}

float AABBColliderComponent::DetectHorizontalCollision(RigidBodyComponent *rigidBody)
{
    // --------------
    // TODO - PARTE 3
    // --------------
    if (mIsStatic) return false;

    auto colliders = mOwner->GetGame()->GetAABBColliders();

    // TODO 4.1: Percorra todos os colliders e verifique se o objeto dono do componente
    //  está colidindo com algum deles. Antes dessa verificação, verifique se o collider está habilitado
    //  e se não é o próprio collider do objeto dono do componente. Em ambos os casos, ignore o collider e
    //  continue a verificação. Se o objeto dono do componente estiver colidindo horizontalmente com algum collider,
    //  resolva a colisão horizontal e retorne o valor da interseção horizontal mínima. Utilize o método
    //  `GetMinHorizontalOverlap` para calcular a interseção horizontal mínima. Utilize o método
    //  `ResolveHorizontalCollisions` para resolver a colisão horizontal. Utilize o método `OnHorizontalCollision`
    //  do objeto dono do componente para notificar a colisão ao Actor dono desse componente.
    for (auto collider : colliders) {
        if (collider->mOwner->GetState() != ActorState::Active) continue;
        if (collider == this) continue;

        if (Intersect(*collider)) {
            SDL_Log("AABBColliderComponent::Intersect");
            auto min_overlap = GetMinHorizontalOverlap(collider);
            if (min_overlap > 0) {
                ResolveHorizontalCollisions(mOwner->GetComponent<RigidBodyComponent>(), min_overlap);
                collider->mOwner->OnHorizontalCollision(min_overlap, this);
            }
        }
    }

    return 0.0f;
}

float AABBColliderComponent::DetectVertialCollision(RigidBodyComponent *rigidBody) {
    // --------------
    // TODO - PARTE 3
    // --------------
    if (mIsStatic) return false;

    auto colliders = mOwner->GetGame()->GetAABBColliders();

    // TODO 4.2: Percorra todos os colliders e verifique se o objeto dono do componente
    //  está colidindo com algum deles. Antes dessa verificação, verifique se o collider está habilitado
    //  e se não é o próprio collider do objeto dono do componente. Em ambos os casos, ignore o collider e
    //  continue a verificação. Se o objeto dono do componente estiver colidindo verticalmente com algum collider,
    //  resolva a colisão vertical e retorne o valor da interseção vertical mínima. Utilize o método
    //  `GetMinVerticalOverlap` para calcular a interseção vertical mínima. Utilize o método
    //  `ResolveVerticalCollisions` para resolver a colisão vertical. Utilize o método `OnVerticalCollision`
    //  do objeto dono do componente para notificar a colisão ao Actor dono desse componente.
    for (auto collider : colliders) {
        if (collider->mOwner->GetState() != ActorState::Active) continue;
        if (collider == this) continue;

        if (Intersect(*collider)) {
            SDL_Log("AABBColliderComponent::Intersect");
            auto min_overlap = GetMinVerticalOverlap(collider);
            if (min_overlap > 0) {
                ResolveVerticalCollisions(mOwner->GetComponent<RigidBodyComponent>(), min_overlap);
                collider->mOwner->OnVerticalCollision(min_overlap, this);
            }
        }
    }

    return 0.0f;
}

void AABBColliderComponent::ResolveHorizontalCollisions(RigidBodyComponent *rigidBody, const float minXOverlap)
{   // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 5.1 (2 linhas): Aplique a resolução de colisão horizontal. Para isso, mova o objeto dono do componente
    //  para o lado oposto da colisão. Para isso, subtraia o valor da interseção horizontal mínima da posição do
    //  objeto dono do componente. Utilize o método `SetPosition` do objeto dono do componente. Altere também a
    //  velocidade horizontal do objeto dono do componente para 0.0f. Utilize o método `SetVelocity`.
    auto actorPosition = mOwner->GetPosition();
    actorPosition.x -= minXOverlap;
    mOwner->SetPosition(actorPosition);
    auto velocity = rigidBody->GetVelocity();
    velocity.x = 0.0f;
    rigidBody->SetVelocity(velocity);


}

void AABBColliderComponent::ResolveVerticalCollisions(RigidBodyComponent *rigidBody, const float minYOverlap)
{
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 5.2 (2 linhas): Aplique a resolução de colisão vertical. Para isso, mova o objeto dono do componente
    //  para o lado oposto da colisão. Para isso, subtraia o valor da interseção vertical mínima da posição do
    //  objeto dono do componente. Utilize o método `SetPosition` do objeto dono do componente. Altere também a
    //  velocidade vertical do objeto dono do componente para 0.0f. Utilize o método `SetVelocity`.
    auto actorPosition = mOwner->GetPosition();
    actorPosition.y -= minYOverlap;
    mOwner->SetPosition(actorPosition);
    auto velocity = rigidBody->GetVelocity();
    velocity.y = 0.0f;
    rigidBody->SetVelocity(velocity);


    // TODO 5.3 (1 linha): Verifique se a colisão vertical ocorreu por cima, ou seja, se o valor da interseção
    //  vertical mínima é maior que 0.0f. Se sim, chame o método `SetOnGround` do objeto dono do componente.
    if (minYOverlap > 0) {
        mOwner->SetOnGround();
    }
}
