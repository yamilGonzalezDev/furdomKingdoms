#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>
#include "texts.hpp"
#include "player.hpp"

enum class EnemyState
{
    ENEMYIDLE,
    ENEMYATTACKING,
    CHASING,
    ENEMYDEAD
};

class Enemy
{
    protected:
        const float PPM = 30.f;
        float _dmg, _hp;
    public:
        virtual ~Enemy() = default;

        virtual void setAnimation(EnemyState) = 0;
        virtual void update(float) = 0;
        virtual void logic(Player*) = 0;
        virtual void loadTextures() = 0;
        virtual void takeDmg(float) = 0;
        virtual void destroy(b2World*) = 0;
        virtual void render(sf::RenderWindow&) = 0;
        virtual float dealDmg() = 0;
        virtual float getHp() { return 2.f; };
        virtual bool getIsAlive() { return true; };
        virtual sf::Sprite getSprite() = 0;
};

class Ghost : public Enemy
{
    private:
        const sf::Vector2i ENEMY_SIZE = {80, 80};
        int currentFrame = 0;
        float elapsedTime = 0.0f, frameDuration = 0.f;
        bool isAlive = true;
        sf::Sprite sprite;
        sf::Texture texture;
        b2Body* body;
        b2Body* enemySensor;
        EnemyState currentState = EnemyState::ENEMYIDLE;
        std::unordered_map<EnemyState, Animation> animations;
        Animation* currentAnimation = nullptr;

        void setAnimation(EnemyState);
    public:
        Ghost(b2World*, float, float);

        void update(float) override;
        void logic(Player*) override;
        void loadTextures() override;
        void takeDmg(float) override;
        void destroy(b2World*) override;
        void render(sf::RenderWindow&) override;
        float dealDmg() override;
        float getHp() { return _hp; };
        bool getIsAlive() override { return isAlive; };
        sf::Sprite getSprite() override;
};

class Skeleton : public Enemy
{
    private:
        int currentFrame = 0;
        float elapsedTime = 0.0f, frameDuration = 0.f;
        bool isAlive = true;
        sf::Sprite sprite;
        sf::Texture texture;
        b2Body* body;
        b2Body* enemySensor;
        EnemyState currentState = EnemyState::ENEMYIDLE;
        std::unordered_map<EnemyState, Animation> animations;
        Animation* currentAnimation = nullptr;

        void setAnimation(EnemyState) override {};
    public:
        Skeleton(b2World*, float, float);

        void update(float) override;
        void logic(Player*) override;
        void loadTextures() override;
        void takeDmg(float) override;
        void destroy(b2World*) override;
        void render(sf::RenderWindow&) override;
        float dealDmg() override;
        float getHp() { return _hp; };
        bool getIsAlive() override { return true; };
        sf::Sprite getSprite() override;
};

class Goblin : public Enemy
{
    private:
        int currentFrame = 0;
        float elapsedTime = 0.0f, frameDuration = 0.f;
        bool isAlive = true;
        sf::Sprite sprite;
        sf::Texture texture;
        b2Body* body;
        b2Body* enemySensor;
        EnemyState currentState = EnemyState::ENEMYIDLE;
        std::unordered_map<EnemyState, Animation> animations;
        Animation* currentAnimation = nullptr;

        void setAnimation(EnemyState) override {};
    public:
        Goblin(b2World*, float, float);

        void update(float) override;
        void logic(Player*) override;
        void loadTextures() override;
        void takeDmg(float) override;
        void destroy(b2World*) override;
        void render(sf::RenderWindow&) override;
        float dealDmg() override;
        float getHp() { return _hp; };
        bool getIsAlive() override { return true; };
        sf::Sprite getSprite() override { return sprite; };
};

class EnemiesFactory
{
    public:
        virtual ~EnemiesFactory() = default;
        virtual Enemy* createEnemy(b2World*, float, float) = 0;
};

class GhostFactory : public EnemiesFactory
{
    Enemy* createEnemy(b2World* world, float x, float y) override
    {
        return new Ghost(world, x, y);
    };
};

class SkeletonFactory : public EnemiesFactory
{
    Enemy* createEnemy(b2World* world, float x, float y) override
    {
        return new Skeleton(world, x, y);
    };
};

class GoblinFactory : public EnemiesFactory
{
    Enemy* createEnemy(b2World* world, float x, float y) override
    {
        return new Goblin(world, x, y);
    };
};

#endif // ENEMY_HPP_INCLUDED
