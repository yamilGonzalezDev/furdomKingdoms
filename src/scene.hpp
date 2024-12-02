#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include "observer.hpp"

enum class SceneState
{
    Menu,
    House,
    City,
    //Forest,
    Default
};

class Scene : public Observer
{
    protected:
        const float WIDTH = 1366;
        const float HEIGHT = 768;
    public:
        virtual ~Scene() = default;

        virtual void update(sf::RenderWindow&, float) = 0;

        virtual void render(sf::RenderWindow&) = 0;

        virtual bool shouldTransition() const { return false; };

        virtual SceneState nextSceneState() const = 0;
};

class MenuScene : public Scene
{
    public:
        MenuScene();

        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void notify(ObserverEvents) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
    private:
        sf::Font font;
        sf::Text menuOptions[3];
        int currentOption = 0;
};

class HouseScene : public Scene
{
    public:
        HouseScene();

        void update(sf::RenderWindow&, float) override;
        void render(sf::RenderWindow&) override;
        void notify(ObserverEvents) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture, candleText, tableText, farTexture, midTexture, nearTexture;
        sf::Sprite background,candle, table, farSprite, midSprite, nearSprite;
        sf::RectangleShape rect;
        void startCinematic(bool);
    private:
        float parallaxFactorFar = 0.05f;
        float parallaxFactorMid = 0.2f;
        float parallaxFactorNear = .5f;
        bool transition = false;
};

class CityScene : public Scene
{
    public:
        CityScene();

        void notify(ObserverEvents) override;
        void update(sf::RenderWindow&, float) override;
        void render(sf::RenderWindow&) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
    private:
};

class ForestScene : public Scene
{
    public:
        ForestScene();

        void notify(ObserverEvents) override;
        void update(sf::RenderWindow&, float) override;
        void render(sf::RenderWindow&) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
    private:
};

#endif // SCENE_HPP_INCLUDED
