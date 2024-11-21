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
        virtual void createBody() = 0;
        virtual void update(float) = 0;
        virtual void loadTextures() = 0;
        virtual void render(sf::RenderWindow&) = 0;
};

class EnemyFactory
{
    public:
        virtual ~EnemyFactory();
        Enemy* createEnemy(b2World*, int, float, float, float, float);
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
        Ghost(b2World*, float, float, float, float);
        void logic() override;
        void createBody() override;
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
        void createBody() override;
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
        void createBody() override;
        void update(float) override;
        void loadTextures() override;
        void render(sf::RenderWindow&) override;
};

#endif // ENEMY_HPP_INCLUDED
