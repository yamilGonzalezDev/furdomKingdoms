#include "player.hpp"
#include <iostream>

Player::Player()
    : isMoving(false), isJumping(false), isOnGround(true), isAttacking(false)
{
    loadTextures();
    animations.emplace(PlayerState::Idle, Animation{
        { sf::IntRect(0, 0, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(50, 0, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(100, 0, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(150, 0, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.15f
    });

    animations.emplace(PlayerState::Running, Animation{
        { sf::IntRect(50, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(100, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(150, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(200, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(250, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(300, 37, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.1f
    });

    animations.emplace(PlayerState::Jumping, Animation{
        { sf::IntRect(0, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(50, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(100, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(150, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(200, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(250, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(300, 74, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(0, 111, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.082f
    });

    animations.emplace(PlayerState::Falling, Animation{
        { sf::IntRect(50, 111, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(100, 111, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.2f
    });

    animations.emplace(PlayerState::Attacking, Animation{
        { sf::IntRect(0, 148, CHARACTER_SIZE.x, CHARACTER_SIZE.y),
          sf::IntRect(50, 148, CHARACTER_SIZE.x, CHARACTER_SIZE.y) }, 0.08f
    });
}

bool Player::loadTextures()
{
    if(!texture.loadFromFile("Textures/mainCharacter/charSheetNeeko.png"))
    {
        return false;
    }

    sprite.setTexture(texture);

    return true;
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
    fixtureDef.filter.maskBits = CATEGORY_LIMITS | CATEGORY_GROUND | CATEGORY_FLOOR;
    playerBody->CreateFixture(&fixtureDef);

    initBody(playerBody, PLAYER, this);

    sprite.setTexture(texture);
    sprite.setScale(1.5f, 1.5f);
    sprite.setTextureRect(animations.at(currentState).frames[0]);

    playerBody->SetFixedRotation(true);
}

void Player::setAnimation(PlayerState state)
{
    auto it = animations.find(state);
    if(it != animations.end())
    {
        currentAnimation = &(it->second);
    }
    currentFrame = 0;
    elapsedTime = 0.f;
}

void Player::switchState(PlayerState state)
{
    if(currentState == state) return;

    currentState = state;

    setAnimation(state);
}

void Player::updateAnimation(float deltaTime)
{
    elapsedTime += deltaTime;
    if(elapsedTime >= animations.at(currentState).frameDuration)
    {
        currentFrame = (currentFrame + 1) % animations.at(currentState).frames.size();
        sprite.setTextureRect(animations.at(currentState).frames[currentFrame]);
        elapsedTime = 0.f;
    }
}

void Player::keyboardInput(float deltaTime)
{
    velocity = playerBody->GetLinearVelocity();

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isAttacking && isOnGround)
    {
        switchState(PlayerState::Attacking);
        return;
    }


    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround && !isJumping)
    {
        velocity.y = -10;
        switchState(PlayerState::Jumping);
        isJumping = true;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocity.x = -MOVE_SPEED;
        sprite.setScale(-1.5f, 1.5f);
        sprite.setOrigin(sprite.getLocalBounds().width, 0.f);
        isMoving = true;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x = MOVE_SPEED;
        sprite.setScale(1.5f, 1.5f);
        sprite.setOrigin(0.f, 0.f);
        isMoving = true;
    }
    else if(!isJumping)
    {
        velocity.x = 0;
        isMoving = false;
    }

    playerBody->SetLinearVelocity(velocity);

    updatePhysics();
}

void Player::updatePhysics()
{
    sf::Vector2f spritePos((playerBody->GetPosition().x * PPM) - 36.f, (playerBody->GetPosition().y * PPM) - 29.f);

    sprite.setPosition(spritePos);

    if(!isOnGround && playerBody->GetLinearVelocity().y > 0)
    {
        switchState(PlayerState::Falling);
    }
    else if(isMoving && !isJumping)
    {
        switchState(PlayerState::Running);
    }
    else if(isOnGround && !isJumping && !isMoving)
    {
        switchState(PlayerState::Idle);
    }
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

//void Player::setFall(){ playerBody->SetLinearVelocity(); }

void Player::setIsOnGround(bool v){ isOnGround = v; }

void Player::setIsJumping(bool v){ isJumping = v; }

PlayerState Player::getPlayerState() const { return currentState; }

sf::Vector2f Player::getPos() const { return sprite.getPosition(); }

bool Player::getIsOnGround() const { return isOnGround; }

void Player::debug() const { return; }

Player::~Player()
{
    if(playerBody && playerBody->GetWorld())
    {
        playerBody->GetWorld()->DestroyBody(playerBody);
    }
}
