#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include "observer.hpp"
#include "player.hpp"

enum class SceneState
{
    Menu,
    House,
    City,
    Bar,
    Default
};

class Scene : public Observer
{
    protected:
        const float WIDTH = 1366;
        const float HEIGHT = 768;
        PlayerAnimations playerAnimations;
        bool drawPlayer;
    public:
        virtual ~Scene() = default;

        virtual void render(sf::RenderWindow&) = 0;

        virtual void update(sf::RenderWindow&, float) = 0;

        virtual void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) {};

        virtual bool shouldTransition() const { return false; };

        virtual SceneState nextSceneState() const = 0;

};

class MenuScene : public Scene
{
    public:
        MenuScene();

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
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

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
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

        void sensorNotify(bool) override;
        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
    private:
        bool transition = false;
        bool sensorActive = false;
};

class ForestScene : public Scene
{
    public:
        ForestScene();

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture backgroundTexture;
        sf::Sprite background;
    private:
};

/*class Castle : public Scene
{
    public:
        Castle();

        void notify(ObserverEvents) override;
        void update(sf::RenderWindow&, float) override;
        void render(sf::RenderWindow&) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
};*/

class BarScene : public Scene
{
    public:
        BarScene();

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        sf::Texture barTexture;
        sf::Sprite bar;
    private:
        bool transition = false;

};

#endif // SCENE_HPP_INCLUDED
