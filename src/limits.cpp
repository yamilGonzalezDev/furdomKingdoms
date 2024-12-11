#include "limits.hpp"
#include "entity.hpp"

Limits::Limits(b2World* world, float x, float y, float width, float height, Kind kind)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x / PPM, y / PPM);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(width / PPM, height / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.friction = 0.f;
    fixtureDef.density = 0.f;
    fixtureDef.filter.categoryBits = CATEGORY_LIMITS;
    fixtureDef.filter.maskBits = 0xFFFF;

    body->CreateFixture(&fixtureDef);

    initBody(body, kind, this);
}

Wall::Wall(b2World* world, float x, float y, float width, float height, Kind kind)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x / PPM, y / PPM);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(width / PPM, height / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.friction = 0.f;
    fixtureDef.density = 0.f;
    fixtureDef.filter.categoryBits = CATEGORY_LIMITS;
    fixtureDef.filter.maskBits = 0xFFFF;

    body->CreateFixture(&fixtureDef);

    initBody(body, kind, this);
}

Sensor::Sensor(b2World* world, float x, float y, float width, float height, Kind kind)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / PPM, y / PPM);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(width / PPM, height / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.isSensor = true;
    fixtureDef.filter.categoryBits = CATEGORY_SENSOR;
    fixtureDef.filter.maskBits = CATEGORY_PLAYER;

    body->CreateFixture(&fixtureDef);

    initBody(body, kind, this);
}

Bounds* LimitsFactory::createBound(b2World* world, float x, float y, float width, float height, Kind kind) const
{
    return new Limits(world, x, y, width, height, kind);
}

Bounds* LimitsFactory::createWall(b2World* world, float x, float y, float width, float height, Kind kind) const
{
    return new Wall(world, x, y, width, height, kind);
}

Bounds* SensorFactory::createBound(b2World* world, float x, float y, float width, float height, Kind kind) const
{
    return new Sensor(world, x, y, width, height, kind);
}
