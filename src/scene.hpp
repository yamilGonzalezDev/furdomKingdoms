#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>
#include "observer.hpp"
#include "player.hpp"
#include "enemy.hpp"

enum class SceneState
{
    MENU,
    HOUSE,
    CITY,
    BAR,
    TEST,
    DEFAULT
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

        virtual void setEnemySprites(const std::vector<Enemy*>& enemiesReferences) {};
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
        sf::Texture candleText, tableText, houseText, bedText, layerTexture1, layerTexture2, layerTexture3, layerTexture4, layerTexture5;
        sf::Sprite candle, table, house, bed, layer1, layer2, layer3, layer4, layer5;
        void startCinematic(bool);
    private:
        float parallaxFactor5 = .04f;
        float parallaxFactor4 = .08f;
        float parallaxFactor3 = .032f;
        float parallaxFactor2 = .018f;
        float parallaxFactor1 = .020f;
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

class TestScene : public Scene
{
    public:
        TestScene();

        void notify(ObserverEvents) override;
        void render(sf::RenderWindow&) override;
        void update(sf::RenderWindow&, float) override;
        void updatePlayer(float, b2Vec2, sf::Vector2f, PlayerState) override;
        bool shouldTransition() const override;
        SceneState nextSceneState() const override;
        void setEnemySprites(const std::vector<Enemy*>&) override;
    private:
        std::vector<Enemy*> enemyReferences;
};

#endif // SCENE_HPP_INCLUDED
