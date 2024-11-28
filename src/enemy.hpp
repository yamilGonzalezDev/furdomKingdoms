#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>
#include "texts.hpp"

enum class EnemyState
{
    IDLE,
    PATROLLING,
    ATTACKING
};

class Enemy
{
    protected:
        const float PPM = 30.f;
    public:
        virtual ~Enemy() = default;

        virtual void logic() = 0;
        virtual void createBody(b2World*, float, float, float, float) = 0;
        virtual void update(float) = 0;
        virtual void loadTextures() = 0;
        virtual void render(sf::RenderWindow&) = 0;
};

class Ghost : public Enemy
{
    private:
        int currentFrame;
        float elapsedTime;
        float frameDuration = 0.f;
        sf::IntRect animations[4];
        sf::Sprite sprite;
        sf::Texture texture;
        b2Body* body;
    public:
        Ghost();
        void logic() override;
        void createBody(b2World*, float, float, float, float) override;
        void update(float) override;
        void loadTextures() override;
        void render(sf::RenderWindow&) override;
};

class Enemy2 : public Enemy
{
    private:
        sf::Sprite sprite;
        sf::Texture texture;
    public:
        void logic() override;
        void createBody(b2World*, float, float, float, float) override;
        void update(float) override;
        void loadTextures() override;
        void render(sf::RenderWindow&) override;
};

class Enemy3 : public Enemy
{
    private:
        sf::Sprite sprite;
        sf::Texture texture;
    public:
        void logic() override;
        void createBody(b2World*, float, float, float, float) override;
        void update(float) override;
        void loadTextures() override;
        void render(sf::RenderWindow&) override;
};

class EnemyFactory
{
    public:
        virtual ~EnemyFactory();
        Enemy* createEnemy(b2World*, int, float, float, float, float);
};

class GhostFactory : public EnemyFactory
{
    Enemy* createEnemy(b2World*, int, float, float, float, float)
    {
        return new Ghost;
    };
};

class Enemy2Factory : public EnemyFactory
{
    Enemy* createEnemy(b2World*, int, float, float, float, float)
    {
        return new Enemy2;
    };
};

class Enemy3Factory : public EnemyFactory
{
    Enemy* createEnemy(b2World*, int, float, float, float, float)
    {
        return new Enemy3;
    };
};

#endif // ENEMY_HPP_INCLUDED
