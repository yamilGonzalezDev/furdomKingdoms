#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <unordered_map>
#include "observer.hpp"

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

    Animation(std::vector<sf::IntRect> frames, float duration) : frames(frames), frameDuration(duration) {};
};

class Player : public Observer
{
    private:
        const sf::Vector2i CHARACTER_SIZE = {50, 37};
        const float PPM = 30.f;
        const float MOVE_SPEED = 10.f;

        PlayerState currentState = PlayerState::Idle;
        int currentFrame = 0;
        bool canJump;
        bool isMoving;
        bool isJumping;
        bool isOnGround;
        bool isAttacking;
        float elapsedTime = 0.0f;

        b2Vec2 velocity;

        std::unordered_map<PlayerState, Animation> animations;
        Animation* currentAnimation = nullptr;

        void switchState(PlayerState);
        void setAnimation(PlayerState);

        sf::RectangleShape playerHitbox;
        sf::Sprite sprite;
        sf::Texture texture;

    public:
        void notify(ObserverEvents) override;
        Player();
        ~Player();
        b2Body* playerBody;
        sf::Sprite test;
        bool loadTextures();
        void update();
        void cinematic();
        void updatePhysics();
        void keyboardInput(float);
        void updateAnimation(float);
        void draw(sf::RenderWindow&);
        void createPlayer(b2World*, float, float);
        void setIsMoving(bool);
        void setIsJumping(bool);
        void setIsOnGround(bool);
        void setFall();

        void getIsMoving() const;
        bool getIsOnGround() const;
        sf::Vector2f getPos() const;
        PlayerState getPlayerState() const;

        void debug()const;
};

#endif // PLAYER_HPP_INCLUDED
