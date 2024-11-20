#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>
#include "texts.hpp"

class Enemy
{
    private:
        const float PPM = 30.f;
    public:
        virtual ~Enemy() = default;

        virtual void logic() = 0;
        virtual void render() = 0;
        virtual void update() = 0;
        virtual void createBody() = 0;
        virtual void loadTextures(std::string&) = 0;
};

class EnemyFactory
{
    public:
        virtual ~EnemyFactory();
        Enemy* createEnemy();
};

class Enemy1 : public Enemy
{
    private:
        sf::Sprite sprite;
        sf::Texture texture;
    public:
        void logic() override;
        void render() override;
        void update() override;
        void createBody() override;
        void loadTextures(std::string&) override;
};

class Enemy2 : public Enemy
{
    private:
        sf::Sprite sprite;
        sf::Texture texture;
    public:
        void logic() override;
        void render() override;
        void update() override;
        void createBody() override;
        void loadTextures(std::string&) override;
};

class Enemy3 : public Enemy
{
    private:
        sf::Sprite sprite;
        sf::Texture texture;
    public:
        void logic() override;
        void render() override;
        void update() override;
        void createBody() override;
        void loadTextures(std::string&) override;
};

#endif // ENEMY_HPP_INCLUDED
