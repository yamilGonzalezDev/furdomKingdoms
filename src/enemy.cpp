#include "enemy.hpp"
#include "entity.hpp"
#include <iostream>

Ghost::Ghost(b2World* world, float x, float y) : currentState(EnemyState::ENEMYIDLE)
{
    _dmg = 20.f;
    _hp = 100.f;

    loadTextures();

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / PPM, y / PPM);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape enemyBox;
    enemyBox.SetAsBox(24.f / PPM, 37.f / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &enemyBox;
    fixtureDef.density = 1.f;
    fixtureDef.friction = .3f;
    fixtureDef.filter.categoryBits = CATEGORY_ENEMY;
    fixtureDef.filter.maskBits = CATEGORY_FLOOR | CATEGORY_LIMITS | CATEGORY_PLAYER | CATEGORY_GROUND | CATEGORY_SWORD;

    body->CreateFixture(&fixtureDef);

    initBody(body, Kind::ENEMY, this);

    ///Sensor
    body->SetFixedRotation(true);
    b2BodyDef enemySensorDef;
    enemySensorDef.type = b2_dynamicBody;
    enemySensorDef.position.Set(x, y);
    enemySensor = world->CreateBody(&enemySensorDef);

    b2PolygonShape sensorBox;
    sensorBox.SetAsBox(400.f / PPM, 50.f / PPM);

    b2FixtureDef sensorFixtureDef;
    sensorFixtureDef.shape = &sensorBox;
    sensorFixtureDef.isSensor = true;
    sensorFixtureDef.filter.categoryBits = CATEGORY_ENEMYSENSOR;
    sensorFixtureDef.filter.maskBits = CATEGORY_PLAYER;

    enemySensor->CreateFixture(&sensorFixtureDef);

    initBody(enemySensor, Kind::ENEMYSENSOR, this);
}

void Ghost::loadTextures()
{
    if(!texture.loadFromFile("Textures/enemy/ghost/ghostSheet.png"))
    {
        std::cerr << "Error al cargar las texturas del ghost" << std::endl;
    }

    animations.emplace(EnemyState::ENEMYIDLE, Animation{
        { sf::IntRect(0, 0, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(80, 0, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(160, 0, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(240, 0, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(320, 0, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(400, 0, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(480, 0, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(560, 0, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(640, 0, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(720, 0, ENEMY_SIZE.x, ENEMY_SIZE.y) }, 0.15f
    });

    animations.emplace(EnemyState::CHASING, Animation{
        { sf::IntRect(0, 80, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(80, 80, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(160, 80, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(240, 80, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(320, 80, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(400, 80, ENEMY_SIZE.x, ENEMY_SIZE.y) }, 0.15f
    });

    animations.emplace(EnemyState::ENEMYDEAD, Animation{
        { sf::IntRect(0, 160, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(80, 160, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(160, 160, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(240, 160, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(320, 160, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(400, 160, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(480, 160, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(560, 160, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(640, 160, ENEMY_SIZE.x, ENEMY_SIZE.y),
          sf::IntRect(720, 160, ENEMY_SIZE.x, ENEMY_SIZE.y) }, 0.15f
    });
    sprite.setTexture(texture);
    sprite.setTextureRect(animations.at(currentState).frames[0]);
    sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
    sprite.setScale(1.f, 1.f);
}

void Ghost::logic(Player* player)
{
    if(currentState == EnemyState::CHASING)
    {
        b2Vec2 playerPos = player->getBody()->GetPosition();
        b2Vec2 enemyPos = body->GetPosition();

        b2Vec2 direction = b2Vec2(playerPos.x - enemyPos.x, playerPos.y - enemyPos.y);
        direction.Normalize();

        float distanceX = fabs(playerPos.x - enemyPos.x);
        float distanceY = fabs(playerPos.y - enemyPos.y);
        float distance = sqrt(distanceX * distanceX + distanceY * distanceY);

        float speed = (distance < 2.0f) ? 2.0f : 5.0f;

        b2Vec2 velocity = direction;
        velocity.x *= speed;
        velocity.y *= speed;

        body->SetLinearVelocity(velocity);
    }
}

sf::Sprite Ghost::getSprite()
{
    return sprite;
}

void Ghost::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Ghost::update(float deltaTime)
{
    b2Vec2 pos = body->GetPosition();

    enemySensor->SetTransform(pos, 0.f);

    sprite.setPosition(pos.x * PPM, pos.y * PPM);

    elapsedTime += deltaTime;

    if(animations.find(currentState) != animations.end())
    {
        if(elapsedTime >= animations.at(currentState).frameDuration)
        {
            currentFrame = (currentFrame + 1) % animations.at(currentState).frames.size();
            sprite.setTextureRect(animations.at(currentState).frames[currentFrame]);
            elapsedTime = 0.f;
        }
    }
    else
    {
        std::cerr << "Animación para el estado " << static_cast<int>(currentState) << " no encontrada." << std::endl;
    }
}

void Ghost::setAnimation(EnemyState state)
{
    if(animations.find(state) != animations.end())
    {
        currentState = state;
    }
    else
    {
        std::cerr << "Intento de establecer una animación para un estado inexistente: " << static_cast<int>(state) << std::endl;
    }
}

void Ghost::takeDmg(float dmg)
{
    _hp -= dmg;

    if(_hp <= 0)
    {
        _hp = 0;

        isAlive = false;
    }
}

float Ghost::dealDmg()
{
    return _dmg;
}

void Ghost::destroy(b2World* world)
{
    if (body != nullptr)
    {
        world->DestroyBody(body);
        body = nullptr;
    }

    if (enemySensor != nullptr)
    {
        world->DestroyBody(enemySensor);
        enemySensor = nullptr;
    }
}
