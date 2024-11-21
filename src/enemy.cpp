#include "enemy.hpp"
#include "entity.hpp"

Enemy* EnemyFactory::createEnemy(b2World* world, int type, float x, float y, float width, float height)
{
    switch(type)
    {
        case 1:
            return new Ghost(world, x, y, width, height);
        default:
            return nullptr;
    }
}

Ghost::Ghost(b2World* world, float x, float y, float width, float height)
{
    loadTextures();
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape enemyBox;
    enemyBox.SetAsBox(width, height);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &enemyBox;
    fixtureDef.density = 1.f;
    fixtureDef.friction = .3f;

    body->CreateFixture(&fixtureDef);

    initBody(body, Kind::ENEMY, this);

    sprite.setTexture(texture);
}

void Ghost::loadTextures()
{
    if(!texture.loadFromFile("Textures/enemy/ghostIdle.png"))
    {
        return;
    }

    animations[0] = sf::IntRect(0, 0, 37, 65);
    animations[0] = sf::IntRect(37, 0, 37, 65);
    animations[0] = sf::IntRect(74, 0, 37, 65);
    animations[0] = sf::IntRect(111, 0, 37, 65);
}

void Ghost::logic()
{

}

void Ghost::render(sf::RenderWindow window)
{
    window.draw(sprite);
}

void Ghost::update(float deltaTime)
{

}

void Ghost::createBody()
{

}

EnemyFactory::~EnemyFactory()
{

}
