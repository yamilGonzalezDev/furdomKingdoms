#include "colisions.hpp"
#include <iostream>
#include "player.hpp"
#include "entity.hpp"
#include "limits.hpp"
#include "enemy.hpp"

void Colision::BeginContact(b2Contact* contact)
{
    b2Fixture* fA = contact->GetFixtureA();
    b2Fixture* fB = contact->GetFixtureB();

    b2Body* bA = fA->GetBody();
    b2Body* bB = fB->GetBody();

    UserdataTag* tagA = reinterpret_cast<UserdataTag*>(bA->GetUserData().pointer);
    UserdataTag* tagB = reinterpret_cast<UserdataTag*>(bB->GetUserData().pointer);

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::FLOOR)
    {
        Player* player = reinterpret_cast<Player*>(tagA->object);
        player->setIsJumping(false);
        player->setIsOnGround(true);
    }

    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::FLOOR)
    {
        Player* player = reinterpret_cast<Player*>(tagB->object);
        player->setIsJumping(false);
        player->setIsOnGround(true);
    }

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::HOUSESENSOR)
    {
        Sensor* sensor = reinterpret_cast<Sensor*>(tagB->object);
        sensor->eventTrigger(ObserverEvents::TRANSITION);    }

    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::HOUSESENSOR)
    {
        Sensor* sensor = reinterpret_cast<Sensor*>(tagA->object);
        sensor->eventTrigger(ObserverEvents::TRANSITION);
    }

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::ENEMY)
    {
        b2Vec2 pushBackForce(-1.5f, 0.0f);
        bB->ApplyLinearImpulseToCenter(pushBackForce, true);
        bB->SetLinearDamping(5.0f);
    }

    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::ENEMY)
    {
        b2Vec2 pushBackForce(1.5f, 0.0f);
        bA->ApplyLinearImpulseToCenter(pushBackForce, true);
        bA->SetLinearDamping(5.0f);
    }

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::BARDOOR)
    {
        Sensor* sensor = reinterpret_cast<Sensor*>(tagA->object);
        sensor->sensorTrigger(true);
    }

    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::BARDOOR)
    {
        Sensor* sensor = reinterpret_cast<Sensor*>(tagA->object);
        sensor->sensorTrigger(true);
    }

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::GAMESENSOR)
    {
        Sensor* sensor = reinterpret_cast<Sensor*>(tagB->object);
        sensor->sensorTrigger(true);
    }

    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::GAMESENSOR)
    {
        Sensor* sensor = reinterpret_cast<Sensor*>(tagA->object);
        sensor->sensorTrigger(true);
    }

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::ENEMYSENSOR)
    {
        Enemy* enemy = reinterpret_cast<Enemy*>(tagB->object);
        enemy->setAnimation(EnemyState::CHASING);
    }

    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::ENEMYSENSOR)
    {
        Enemy* enemy = reinterpret_cast<Enemy*>(tagA->object);
        enemy->setAnimation(EnemyState::CHASING);
    }

    if(fA->GetFilterData().categoryBits == CATEGORY_ENEMY && fB->GetFilterData().categoryBits == CATEGORY_SWORD)
    {
        b2Vec2 pushBackForce(10.5f, 0.0f);
        bA->ApplyLinearImpulseToCenter(pushBackForce, true);
        bA->SetLinearDamping(5.0f);

        Enemy* enemy = reinterpret_cast<Enemy*>(tagA->object);
        Player* player = reinterpret_cast<Player*>(tagB->object);

        enemy->takeDmg(player->dealDamage());
    }

    if(fB->GetFilterData().categoryBits == CATEGORY_ENEMY && fA->GetFilterData().categoryBits == CATEGORY_SWORD)
    {
        b2Vec2 pushBackForce(10.5f, 0.0f);
        bB->ApplyLinearImpulseToCenter(pushBackForce, true);
        bB->SetLinearDamping(5.0f);

        Enemy* enemy = reinterpret_cast<Enemy*>(tagA->object);
        Player* player = reinterpret_cast<Player*>(tagB->object);

        enemy->takeDmg(player->dealDamage());
    }
}

void Colision::EndContact(b2Contact* contact)
{
    b2Fixture* fA = contact->GetFixtureA();
    b2Fixture* fB = contact->GetFixtureB();

    b2Body* bA = fA->GetBody();
    b2Body* bB = fB->GetBody();

    UserdataTag* tagA = reinterpret_cast<UserdataTag*>(bA->GetUserData().pointer);
    UserdataTag* tagB = reinterpret_cast<UserdataTag*>(bB->GetUserData().pointer);

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::FLOOR)
    {
        Player* player = reinterpret_cast<Player*>(tagA->object);
        player->setIsJumping(true);
        player->setIsOnGround(false);
    }
    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::FLOOR)
    {
        Player* player = reinterpret_cast<Player*>(tagB->object);
        player->setIsJumping(true);
        player->setIsOnGround(false);
    }

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::BARDOOR)
    {
        Sensor* sensor = reinterpret_cast<Sensor*>(tagA->object);
        sensor->sensorTrigger(false);
    }

    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::BARDOOR)
    {
        Sensor* sensor = reinterpret_cast<Sensor*>(tagA->object);
        sensor->sensorTrigger(false);
    }

    if(tagA->kind == Kind::PLAYER && tagB->kind == Kind::ENEMYSENSOR)
    {
        Enemy* enemy = reinterpret_cast<Enemy*>(tagB->object);
        enemy->setAnimation(EnemyState::CHASING);
    }

    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::ENEMYSENSOR)
    {
        Enemy* enemy = reinterpret_cast<Enemy*>(tagA->object);
        enemy->setAnimation(EnemyState::CHASING);
    }
}

void Colision::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    b2Fixture* fA = contact->GetFixtureA();
    b2Fixture* fB = contact->GetFixtureB();

    b2Body* bA = fA->GetBody();
    b2Body* bB = fB->GetBody();

    UserdataTag* tagA = reinterpret_cast<UserdataTag*>(bA->GetUserData().pointer);
    UserdataTag* tagB = reinterpret_cast<UserdataTag*>(bB->GetUserData().pointer);

    if((tagA->kind == Kind::PLAYER && tagB->kind == Kind::ENEMY) || (tagB->kind == Kind::PLAYER && tagA->kind == Kind::ENEMY))
    {
        contact->SetFriction(0.f);
    }

    if((tagA->kind == Kind::PLAYER && tagB->kind == Kind::WALLS) || (tagB->kind == Kind::PLAYER && tagA->kind == Kind::WALLS))
    {
        contact->SetFriction(0.f);
    }
}

void Colision::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}
