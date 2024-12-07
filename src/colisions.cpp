#include "colisions.hpp"
#include <iostream>
#include "player.hpp"
#include "entity.hpp"
#include "limits.hpp"

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
        std::cout << "Enemigo detectado" << std::endl;
    }
    if(tagB->kind == Kind::PLAYER && tagA->kind == Kind::ENEMY)
    {
        std::cout << "Enemigo detectado" << std::endl;
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
}
