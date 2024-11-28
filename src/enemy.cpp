#include "enemy.hpp"
#include "entity.hpp"
#include <iostream>

Enemy* EnemyFactory::createEnemy(b2World* world, int type, float x, float y, float width, float height)
{
    switch(type)
    {
        case 1:
            std::cout << "Ghost created" << std::endl;
            return new Ghost;
        default:
            return nullptr;
    }
}

Ghost::Ghost(/*b2World* world, float x, float y, float width, float height*/)
{
    /*createBody(b2World* world, float x, float y, float width, float height);*/

    loadTextures();

    sprite.setTexture(texture);
    sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
    sprite.setTextureRect(animations[0]);
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

void Ghost::render(sf::RenderWindow& window)
{
    b2Vec2 pos = body->GetPosition();

    sprite.setPosition(pos.x, pos.y);
    window.draw(sprite);
}

void Ghost::update(float deltaTime)
{
    elapsedTime += deltaTime;
    if(elapsedTime > frameDuration)
    {
        currentFrame = (currentFrame + 1) % 4;
        sprite.setTextureRect(animations[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Ghost::createBody(b2World* world, float x, float y, float width, float height)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / PPM, y / PPM);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape enemyBox;
    enemyBox.SetAsBox(width / PPM, height / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &enemyBox;
    fixtureDef.density = 1.f;
    fixtureDef.friction = .3f;
    fixtureDef.filter.categoryBits = CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = CATEGORY_FLOOR | CATEGORY_LIMITS | CATEGORY_PLAYER | CATEGORY_GROUND;

    body->CreateFixture(&fixtureDef);

    initBody(body, Kind::ENEMY, this);

    body->SetFixedRotation(true);
}

EnemyFactory::~EnemyFactory()
{

}
