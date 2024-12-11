#ifndef DIRECTOR_HPP_INCLUDED
#define DIRECTOR_HPP_INCLUDED

#include <memory>
#include "enemy.hpp"
#include "npc.hpp"
#include "player.hpp"
#include "limits.hpp"
#include "colisions.hpp"
#include "scene.hpp"
#include "fooDraw.hpp"
#include "observer.hpp"

enum class TransitionState
{
    NONE,
    FADINGOUT,
    LOADING,
    FADINGIN
};

class Director
{
    public:
        Director();
        ~Director();
        void run();
    private:
        const float WIDTH;
        const float HEIGHT;
        const float timeStep = 1.f/60.f;
        const float FLOORHEIGHT = 528.f;
        const int velocityIterations = 6;
        const int positionIterations = 2;
        const float speed = 200.f;
        sf::RenderWindow window;
        sf::View view;
        sf::Clock clock;
        FooDraw fooDrawInstance;
        SceneState nextScene;
        TransitionState transitionState;
        bool loaded, fadingIn, fadingOut, drawNpcs, drawPlayer, changeScene, drawEnemies, transitioning;
        int alpha = 255;
        float transitionTime = 0.f;
        float maxTransitionTime;
        sf::RectangleShape fadeRectangle, center;

        sf::Texture testText;
        sf::Sprite spriteTest;

        b2World* world = nullptr;

        std::vector<Enemy*> enemies;
        std::unique_ptr<BoundsFactory> boundFactory;
        std::unique_ptr<EnemiesFactory> enemyFactory;
        Bounds* sensor = nullptr;
        Enemy* enemy = nullptr;
        Player* player = nullptr;
        Scene* currentScene = nullptr;
        Colision* colisionCheck = nullptr;

        void update(float);
        void updateScene(float);
        void gameEvents();
        void render();

        void initWorld(float);
        void initCityScene();
        void initMenuScene();
        void initHouseScene();
        void initBarScene();
        void initTestScene();
        void setScene(Scene*);
        void cleanScene(b2World*);
        void fadeOut(float);
        void fadeIn(float);
        void Garcha(b2World* world);

        b2Vec2 getMousePositionInWorld(const sf::RenderWindow&);
};

#endif // DIRECTOR_HPP_INCLUDED
