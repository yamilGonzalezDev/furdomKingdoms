#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include "observer.hpp"

class Scene : public Observer
{
    protected:
        const float WIDTH = 1366;
        const float HEIGHT = 768;
    public:
        virtual ~Scene(){}

        virtual void init() = 0;

        virtual void update(sf::RenderWindow&, float) = 0;

        virtual void render(sf::RenderWindow&) = 0;

        virtual void cleanup() = 0;

        virtual bool shouldTransition() = 0;
};

class MenuScene : public Scene
{
    public:
        void init() override;
        void cleanup() override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void notify(ObserverEvents) override;
        bool shouldTransition() override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
    private:
        sf::Font font;
        sf::Text menuOptions[3];
        int currentOption;
};

class HouseScene : public Scene
{
    public:
        void init() override;
        void update(sf::RenderWindow&, float) override;
        void render(sf::RenderWindow&) override;
        void cleanup() override;
        void notify(ObserverEvents) override;
        bool shouldTransition() override;
        sf::Texture backgroundTexture, candleText, tableText, farTexture, midTexture, nearTexture;
        sf::Sprite background,candle, table, farSprite, midSprite, nearSprite;
        sf::RectangleShape rect;
        void startCinematic(bool);
    private:
        float parallaxFactorFar = 0.05f;
        float parallaxFactorMid = 0.2f;
        float parallaxFactorNear = .5f;
        bool cinematic;
};

class CityScene : public Scene
{
    public:
        void notify(ObserverEvents) override;
        void init() override;
        void update(sf::RenderWindow&, float) override;
        void render(sf::RenderWindow&) override;
        void cleanup() override;
        bool shouldTransition() override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
    private:
};

#endif // SCENE_HPP_INCLUDED
