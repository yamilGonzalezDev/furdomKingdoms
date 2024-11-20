#include "limits.hpp"
#include "entity.hpp"

void Ground::createGround(b2World* world, float x, float y, float width, float height)
{
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(x / PPM, y / PPM);

    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(width / PPM, height / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &groundBox;
    fixtureDef.density = 0.f;
    fixtureDef.friction = 0.4f;
    fixtureDef.filter.categoryBits = CATEGORY_GROUND;
    fixtureDef.filter.maskBits = 0xFFFF;

    groundBody->CreateFixture(&fixtureDef);

    initBody(groundBody, Kind::FLOOR, this);
}

void Limits::createWall(b2World* world, float x, float y, float width, float height)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x / PPM, y / PPM);
    b2Body* body = world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(width / PPM, height / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 0.f;
    fixtureDef.friction = 0.01f;
    fixtureDef.filter.categoryBits = CATEGORY_LIMITS;
    fixtureDef.filter.maskBits = 0xFFFF;

    body->CreateFixture(&fixtureDef);

    initBody(body, Kind::WALLS, this);
}

void Limits::createFloor(b2World* world, float x, float y, float width, float height)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x / PPM, y / PPM);
    b2Body* body = world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(width / PPM, height / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 0.f;
    fixtureDef.friction = 0.f;
    fixtureDef.filter.categoryBits = CATEGORY_LIMITS;
    fixtureDef.filter.maskBits = 0xFFFF;

    body->CreateFixture(&fixtureDef);

    initBody(body, Kind::FLOOR, this);
}
