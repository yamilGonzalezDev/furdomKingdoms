#include <iostream>
#include "director.hpp"
#include "colisions.hpp"
#include "entity.hpp"

void debugA(std::string a)
{
    ///std::cout << "Mensaje de " + a << std::endl;
}

Director::Director() : WIDTH(1366), HEIGHT(768), fooDrawInstance(window)
{
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Furdom Kingdoms");
    window.setFramerateLimit(60);
    nextScene = SceneState::City;
    loaded = false;
    drawNpcs = false;
    cargando = false;
    aclarando = false;
    drawPlayer = false;
    oscureciendo = true;
    drawEnemies = false;
    transitioning = true;
    transitionState = TransitionState::LOADING;
    view.setSize(/*window.getSize().x, window.getSize().y*/window.getSize().x * 0.5f, window.getSize().y * 0.5f);
    alpha = 0;
    fadeRectangle.setSize(sf::Vector2f(WIDTH, HEIGHT));
    fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
}

void Director::run() ///buclePrincipal();
{
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            b2Body* body = player->getBody();

            std::cout << "x: " << body->GetPosition().x << ", y: " << body->GetPosition().y << std::endl;
        }

        debugA("Comienzo");
        updateScene(deltaTime);
        debugA("UpdateScene");
        if(!transitioning)
        {
            update(deltaTime);
            debugA("update");
            gameEvents();
            debugA("gameEvents");
        }
        render();
        debugA("render");
    }
}

void Director::update(float deltaTime)
{
    if(drawPlayer && player != nullptr)
    {
        if(window.hasFocus())
        {
            player->updatePhysics();
            player->updateAnimation(deltaTime);
            player->keyboardInput();
        }
    }
    debugA("player");
    if(drawNpcs)
    {
    }
    debugA("npcs");
    if(drawEnemies)
    {
    }
    debugA("enemy");
}

void Director::updateScene(float deltaTime)
{
    if(!transitioning && currentScene != nullptr && currentScene->shouldTransition())
    {
        transitioning = true;
        nextScene = currentScene->nextSceneState();
        transitionState = TransitionState::FADINGOUT;
    }

    if(transitioning)
    {
        switch(transitionState)
        {
            case TransitionState::FADINGOUT:
                fadeOut(deltaTime);
                break;
            case TransitionState::LOADING:
                switch(nextScene)
                {
                    case SceneState::Menu:
                        initMenuScene();
                        break;
                    case SceneState::House:
                        initHouseScene();
                        break;
                    case SceneState::City:
                        initCityScene();
                        break;
                    case SceneState::Default:
                        break;
                }
                transitionState = TransitionState::FADINGIN;
                break;
            case TransitionState::FADINGIN:
                fadeIn(deltaTime);
                break;
            case TransitionState::NONE:
                break;
            default:
                break;
        }
    }
    else if(currentScene != nullptr)
    {
        currentScene->update(window, deltaTime);
    }
}

void Director::gameEvents()
{
    if(world != nullptr)
    {
        world->Step(timeStep, velocityIterations, positionIterations);
    }
}

void Director::render()
{
    window.clear();
    if(currentScene != nullptr)
    {
        currentScene->render(window);
    }
    if(drawNpcs)
    {
    }
    if(drawPlayer && player != nullptr)
    {
        view.setCenter(player->getPos().x + 33.f, 570);
        window.setView(view);
        player->draw(window);
    }
    if(world)
    {
        world->DebugDraw();
    }
    if(drawEnemies)
    {
    }
    if(transitioning)
    {
        window.draw(fadeRectangle);
    }

    window.display();
}

void Director::initMenuScene()
{
    if(world != nullptr)
    {
        delete world;
        world = nullptr;
    }
    drawNpcs = false;
    drawPlayer = false;
    drawEnemies = true;

    setScene(new MenuScene);
}

void Director::initHouseScene()
{
    initWorld();

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 1000.f, 670.f, 100.f, 50.f, Kind::HOUSESENSOR);

    setScene(new HouseScene);

    sensor->addObserver(currentScene);

    drawPlayer = true;

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createWall(world, 361.f, 656.f, 8.f, 64.f, Kind::WALLS);

    boundFactory->createWall(world, 791.f, 622.f, 8.f, 32.f, Kind::WALLS);

    boundFactory->createWall(world, 576.f, 560.f, 272.f, 32.f, Kind::LIMITS);

    boundFactory->createBound(world, 577.f, 712.f, 223.f, 8.f, Kind::FLOOR);

    boundFactory->createBound(world, 0.f, 723.f, 3000.f, 0.f, Kind::FLOOR);

    player = new Player();
    player->createPlayer(world, 400.f, 680.f);
}

void Director::initCityScene()
{
    initWorld();

    setScene(new CityScene);

    boundFactory = std::make_unique<SensorFactory>();

    sensor = boundFactory->createBound(world, 137.f, 672.f, 20.f, 28.f, Kind::DOOR);

    boundFactory = std::make_unique<LimitsFactory>();

    boundFactory->createBound(world, 100.f, 100.f, 100.f, 100.f, Kind::LIMITS);

    boundFactory->createBound(world, 650.f, 701.f, 1000.f, 0.f, Kind::FLOOR);

    drawPlayer = true;

    if(player != nullptr)
    {
        player->playerBody->SetTransform(b2Vec2(0.f / 30.f, 676.f / 30.f), 0.0f);
    }
    else
    {
        player = new Player;
        player->createPlayer(world, 4.f, 676.f);
    }
}

void Director::initWorld()
{
    if(world != nullptr)
    {
        cleanScene(world);
    }
    else
    {
        world = new b2World(b2Vec2(0.f, 10.f));
    }

    if(colisionCheck == nullptr)
    {
        colisionCheck = new Colision();
    }

    world->SetContactListener(colisionCheck);

    fooDrawInstance.SetFlags(b2Draw::e_shapeBit);

    world->SetDebugDraw(&fooDrawInstance);
}

void Director::loadScene()
{

}

void Director::setScene(Scene* newScene)
{
    if(currentScene)
    {
        delete currentScene;
        currentScene = nullptr;
    }
    currentScene = newScene;
}

void Director::cleanScene(b2World* world)
{
    if(world != nullptr)
    {
        for(b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
        {
            UserdataTag* tag = reinterpret_cast<UserdataTag*>(body->GetUserData().pointer);

            if(body == player->getBody())
            {
                continue;
            }
            else
            {
                delete tag;
                world->DestroyBody(body);
            }
        }
    }
}

void Director::fadeOut(float deltaTime)
{
    if(alpha < 255)
    {
        alpha += 200 * deltaTime;
        if(alpha > 255)
        {
            alpha = 255;
        }
        fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    }
    else
    {
        transitionState = TransitionState::LOADING;
    }
}

void Director::fadeIn(float deltaTime)
{
    if(alpha > 0)
    {
        alpha -= 200 * deltaTime;
        if(alpha < 0)
        {
            alpha = 0;
        }
        fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
    }
    else
    {
        transitionState = TransitionState::NONE;
        transitioning = false;
    }
}

Director::~Director()
{
    if(player != nullptr)
    {
        delete player;
        player = nullptr;
    }
    if(colisionCheck != nullptr)
    {
        delete colisionCheck;
        colisionCheck = nullptr;
    }
    if(sensor != nullptr)
    {
        delete sensor;
        sensor = nullptr;
    }
    if(currentScene != nullptr)
    {
        delete currentScene;
        currentScene = nullptr;
    }
    if(world != nullptr)
    {
        if(world->GetBodyCount() != 0)
        {
            for(b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
            {
                UserdataTag* tag = reinterpret_cast<UserdataTag*>(body->GetUserData().pointer);
                delete tag;
                world->DestroyBody(body);
            }
            delete world;
        }
        world = nullptr;
    }
    std::cout << "Finalizo correctamente" << std::endl;
}
