#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <unordered_map>
#include "observer.hpp"

const float PPM = 30.f;

enum class PlayerState
{
    Idle,
    Running,
    Jumping,
    Falling,
    Attacking
};

struct Animation
{
    std::vector<sf::IntRect> frames;
    float frameDuration;

    Animation() {};

    Animation(std::vector<sf::IntRect> frames, float duration)  : frames(frames), frameDuration(duration){};
};

class Player
{
    public:
        Player();
        ~Player();
        b2Body* playerBody;
        void keyboardInput();
        void createPlayer(b2World*, float, float);

        void setIsMoving(bool);
        void setIsJumping(bool);
        void setIsOnGround(bool);

        b2Body* getBody();
        b2Vec2 getPos() const;
        sf::Vector2f getScale() const;
        PlayerState getPlayerState() const;
    private:
        const float MOVE_SPEED = 10.f;
        PlayerState currentState = PlayerState::Idle;
        bool isMoving;
        bool isJumping;
        bool isOnGround;
        bool isAttacking;
        b2Vec2 velocity;
        sf::Vector2f spriteScale;
        void switchState(PlayerState);
};

class PlayerAnimations
{
    public:
        PlayerAnimations();
        void draw(sf::RenderWindow&);
        void update(float, b2Vec2, sf::Vector2f, PlayerState);
    private:
        const sf::Vector2i CHARACTER_SIZE = {50, 37};
        sf::Sprite sprite;
        sf::Texture texture;
        std::unordered_map<PlayerState, Animation> animations;
        Animation* currentAnimation = nullptr;
        PlayerState currentState = PlayerState::Idle;
        int currentFrame = 0;
        float elapsedTime = 0.0f;
        void setAnimation(PlayerState);
};

#endif // PLAYER_HPP_INCLUDED
