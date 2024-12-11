#include "player.hpp"
#include "entity.hpp"
#include <iostream>

Player::Player()
    : _hp(100.f), _dmg(10.f), _armor(20.f), isMoving(false), isJumping(false),
    isOnGround(true), isAttacking(false), canBeDamaged(true), isAlive(true)
{

}

void Player::createPlayer(b2World* world, float posX, float posY)
{
    b2BodyDef playerBodyDef;
    playerBodyDef.type = b2_dynamicBody;
    playerBodyDef.position.Set(posX / PPM, posY / PPM);
    playerBody = world->CreateBody(&playerBodyDef);

    b2PolygonShape playerBox;
    playerBox.SetAsBox(17.f / PPM, 24.f / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &playerBox;
    fixtureDef.density = 1.f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = CATEGORY_PLAYER;
    fixtureDef.filter.maskBits = CATEGORY_LIMITS | CATEGORY_GROUND | CATEGORY_FLOOR | CATEGORY_SENSOR | CATEGORY_ENEMYSENSOR | CATEGORY_ENEMY;
    playerBody->CreateFixture(&fixtureDef);

    initBody(playerBody, Kind::PLAYER, this);

    playerBody->SetFixedRotation(true);
}

void Player::switchState(PlayerState state)
{
    if(currentState == state) return;

    currentState = state;
}

void Player::timers(b2World* world, float deltaTime)
{
    if(isAttacking)
    {
        elapsedTime += deltaTime;
        if(elapsedTime >= cooldown)
        {
            isAttacking = false;
            elapsedTime = 0.f;
            if(sword != nullptr)
            {
                world->DestroyBody(sword);
                sword = nullptr;
            }
        }
    }
    if(!canBeDamaged)
    {
        elapsedDamageTime += deltaTime;
        if(elapsedDamageTime >= damageCooldown)
        {
            canBeDamaged = true;
            elapsedDamageTime = 0.f;
        }
    }
}

void Player::keyboardInput(b2World* world)
{
    velocity = playerBody->GetLinearVelocity();

    if(isAttacking)
    {
        velocity = {0.f, 0.f};
        playerBody->SetLinearVelocity(velocity);
        return;
    }

    if(!isAttacking)
    {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && isOnGround)
        {
            switchState(PlayerState::ATTACKING);
            isAttacking = true;
            if(sword == nullptr)
            {
                createAttackHitbox(world);
            }
            return;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround && !isJumping)
        {
            velocity.y = -10;
            switchState(PlayerState::JUMPING);
            isJumping = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            velocity.x = -MOVE_SPEED;
            spriteScale = {-1.5, 1.5};
            setIsMoving(true);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            velocity.x = MOVE_SPEED;
            spriteScale = {1.5f, 1.5f};
            setIsMoving(true);
        }
        else
        {
            velocity.x = 0.f;
            setIsMoving(false);
        }

        if (velocity.y > 0 && !isOnGround)
        {
            switchState(PlayerState::FALLING);
        }
        else if (!isMoving && isOnGround && !isJumping)
        {
            switchState(PlayerState::IDLE);
        }
        else if (isMoving && isOnGround && !isJumping)
        {
            switchState(PlayerState::RUNNING);
        }

        playerBody->SetLinearVelocity(velocity);
    }
}

void Player::takeDmg(float inputDmg)
{
    if(canBeDamaged)
    {
        unsigned int totalDamage = _armor - inputDmg;

        _hp -= totalDamage;

        if(_hp <= 0)
        {
            _hp = 0;
            switchState(PlayerState::DEATH);
        }
    }
}

void Player::createAttackHitbox(b2World* world)
{
    b2Vec2 swordPos = playerBody->GetPosition();

    if(spriteScale.x > 0)
    {
        swordPos.x += 20.f / PPM;
    }
    else
    {
        swordPos.x -= 20.f / PPM;
    }

    b2BodyDef swordBodyDef;
    swordBodyDef.type = b2_dynamicBody;
    swordBodyDef.position.Set(swordPos.x, swordPos.y);
    sword = world->CreateBody(&swordBodyDef);

    b2PolygonShape attackBox;
    attackBox.SetAsBox(15.f / PPM, 20.f / PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &attackBox;
    fixtureDef.density = 0.f;
    fixtureDef.friction = 0.f;
    fixtureDef.filter.categoryBits = CATEGORY_SWORD;
    fixtureDef.filter.maskBits = CATEGORY_ENEMY;

    sword->CreateFixture(&fixtureDef);

    sword->SetBullet(true);

    initBody(sword, Kind::PLAYERSWORD, this);
}

void Player::setIsOnGround(bool v){ isOnGround = v; }

void Player::setIsJumping(bool v){ isJumping = v; }

void Player::setIsMoving(bool v) { isMoving = v; }

b2Vec2 Player::getPos() const { return playerBody->GetPosition(); }

sf::Vector2f Player::getScale() const { return spriteScale; }

PlayerState Player::getPlayerState() const { return currentState; }

b2Body* Player::getBody() { if(playerBody) return playerBody; else return nullptr; }

float Player::getHp() { return _hp; };

void Player::setHp(float valor) { _hp = valor; };

float Player::dealDamage() { return _dmg; }

void Player::setCanBeDamaged(bool v) { canBeDamaged = v; }

/**PlayerAnimations**/

PlayerAnimations::PlayerAnimations()
{
    if(!texture.loadFromFile("Textures/mainCharacter/charSheetNeeko.png"))
    {
        std::cerr << "Error al cargar las texturas del player" << std::endl;
    }

    sprite.setTexture(texture);

    animations.emplace(PlayerState::IDLE, Animation{
        { sf::IntRect(0, 0, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(50, 0, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(100, 0, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(150, 0, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.15f
    });

    animations.emplace(PlayerState::RUNNING, Animation{
        { sf::IntRect(50, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(100, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(150, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(200, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(250, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(300, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.1f
    });

    animations.emplace(PlayerState::JUMPING, Animation{
        { sf::IntRect(0, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(50, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(100, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(150, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(200, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(250, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(300, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(0, 111, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.082f
    });

    animations.emplace(PlayerState::FALLING, Animation{
        { sf::IntRect(50, 111, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(100, 111, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.2f
    });

    animations.emplace(PlayerState::ATTACKING, Animation{
        { sf::IntRect(0, 222, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(50, 222, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(100, 222, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(150, 222, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(200, 222, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.08f
    });

    animations.emplace(PlayerState::DEATH, Animation{
        { sf::IntRect(300, 296, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(0, 333, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(50, 333, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(100, 333, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(150, 333, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(200, 333, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(250, 333, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.08f
    });

    sprite.setTextureRect(animations.at(currentState).frames[0]);
    sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
    sprite.setScale(1.5f, 1.5f);
}

void PlayerAnimations::setAnimation(PlayerState state)
{
    if(currentState == PlayerState::DEATH) return;

    currentState = state;
    auto it = animations.find(currentState);
    if(it != animations.end())
    {
        currentAnimation = &(it->second);
        currentFrame = 0;
        elapsedTime = 0.0f;
    }
}

void PlayerAnimations::update(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    sprite.setPosition(pos.x * PPM, pos.y * PPM);

    sprite.setScale(spriteScale);

    if(currentState != state)
    {
        setAnimation(state);
    }

    elapsedTime += deltaTime;

    if(elapsedTime >= animations.at(currentState).frameDuration)
    {
        if(currentState == PlayerState::DEATH && currentFrame == animations.at(currentState).frames.size() - 1)
        {
            gameOver = true;
            return;
        }
        currentFrame = (currentFrame + 1) % animations.at(currentState).frames.size();
        sprite.setTextureRect(animations.at(currentState).frames[currentFrame]);
        elapsedTime = 0.f;
    }

}

void PlayerAnimations::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
