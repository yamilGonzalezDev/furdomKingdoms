#include "player.hpp"
#include "entity.hpp"
#include <iostream>

Player::Player()
    : isMoving(false), isJumping(false), isOnGround(true), isAttacking(false)
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
    fixtureDef.filter.maskBits = CATEGORY_LIMITS | CATEGORY_GROUND | CATEGORY_FLOOR | CATEGORY_SENSOR;
    playerBody->CreateFixture(&fixtureDef);

    initBody(playerBody, Kind::PLAYER, this);

    playerBody->SetFixedRotation(true);
}

void Player::switchState(PlayerState state)
{
    if(currentState == state) return;

    currentState = state;
}

void Player::keyboardInput()
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
        switchState(PlayerState::Falling);
    }
    else if (!isMoving && isOnGround && !isJumping)
    {
        switchState(PlayerState::Idle);
    }
    else if (isMoving && isOnGround && !isJumping)
    {
        switchState(PlayerState::Running);
    }

    playerBody->SetLinearVelocity(velocity);
}

void Player::setIsOnGround(bool v){ isOnGround = v; }

void Player::setIsJumping(bool v){ isJumping = v; }

void Player::setIsMoving(bool v) { isMoving = v; }

PlayerState Player::getPlayerState() const { return currentState; }

b2Vec2 Player::getPos() const { return playerBody->GetPosition(); }

sf::Vector2f Player::getScale() const { return spriteScale; }

b2Body* Player::getBody() { if(playerBody) return playerBody; else return nullptr; }

Player::~Player()
{

}

/**PlayerAnimations**/

PlayerAnimations::PlayerAnimations()
{
    if(!texture.loadFromFile("Textures/mainCharacter/charSheetNeeko.png"))
    {
        std::cerr << "Error al cargar las texturas del player" << std::endl;
    }

    sprite.setTexture(texture);

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
    sprite.setScale(1.5f, 1.5f);
    sprite.setTextureRect(animations.at(currentState).frames[0]);
    sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
}

void PlayerAnimations::setAnimation(PlayerState state)
{
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

    if (currentState != state)
    {
        setAnimation(state);
    }

    elapsedTime += deltaTime;

    if(elapsedTime >= animations.at(currentState).frameDuration)
    {
        currentFrame = (currentFrame + 1) % animations.at(currentState).frames.size();
        sprite.setTextureRect(animations.at(currentState).frames[currentFrame]);
        elapsedTime = 0.f;
    }
}

void PlayerAnimations::draw(sf::RenderWindow& window)
{
    /*sf::View view;

    view.setCenter(sprite.getPosition());

    window.setView(view);*/
    window.draw(sprite);
}
