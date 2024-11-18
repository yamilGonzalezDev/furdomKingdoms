#include <iostream>
#include "director.hpp"

Director::Director() : fooDrawInstance(window)
{
    init();
}

void Director::run() ///buclePrincipal();
{
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        updateScene(deltaTime);
        update(deltaTime);
        gameEvents(deltaTime);
        render();
    }
}

Director::~Director()
{
    //delete npc;
    delete world;
    delete player;
    delete ground;
    delete collisionCheck;
}

void Director::init()
{
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Furdom Kingdoms");
    window.setFramerateLimit(60);
    currentSceneState = SceneState::MainMenu;
    drawPlayer = false;
    transitioning = false;
    window.setFramerateLimit(60);
    currentScene = nullptr;
    view.setSize(/*window.getSize().x, window.getSize().y*/window.getSize().x * 0.5f, window.getSize().y * 0.5f);
    alpha = 0;
    oscureciendo = true;
    aclarando = false;
    cargando = false;
    fadeRectangle.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
}

void Director::initMenuScene()
{
    if(world != nullptr) world = nullptr;
    if(player != nullptr) player = nullptr;
    drawPlayer = false;
    drawNpcs = false;

    currentScene = new MenuScene;
    currentScene->init();

    sf::Texture texture;
    sf::Sprite sprite;

    if(!texture.loadFromFile("Textures/menu/scroll.png"))
    {
        return;
    }

    sprite.setTexture(texture);
    sprite.setScale(5.f, 5.f);
}

void Director::initHouseScene()
{
    if(currentScene != nullptr) currentScene = nullptr;

    currentScene = new HouseScene;

    currentScene->init();

    b2Body* sensorBody;

    drawPlayer = true;

    world = new b2World(b2Vec2(0.f, 10.f));
    collisionCheck = new Collision();
    world->SetContactListener(collisionCheck);

    fooDrawInstance.SetFlags( b2Draw::e_shapeBit );

    world->SetDebugDraw( &fooDrawInstance );

    createSensor(world, sensorBody, 1000.f, 506.f);

    ground = new Ground;
    ground->createGround(world, 0.f, 723.f, 3000.f, 0.f);

    Bounds bounds;

    bounds.createWall(world, 361.f, 656.f, 8.f, 64.f);

    bounds.createWall(world, 791.f, 597.f, 8.f, 56.f);

    bounds.createFloor(world, 577.f, 712.f, 223.f, 8.f);

    player = new Player;
    player->createPlayer(world, 400.f, 658.f);
}

void Director::initCityScene()
{
    if(currentScene != nullptr) currentScene = nullptr;

    currentScene = new CityScene;

    currentScene->init();

    drawNpcs = true;
    drawPlayer = true;
}

void Director::createSensor(b2World* world, b2Body*& body, float x, float y)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / 30.f, y / 30.f);
    body = world->CreateBody(&bodyDef);
    b2PolygonShape objectBox;
    objectBox.SetAsBox(100.f / 30.f, 200.f / 30.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &objectBox;
    fixtureDef.isSensor = true;
    fixtureDef.filter.categoryBits = CATEGORY_SENSOR;
    fixtureDef.filter.maskBits = CATEGORY_PLAYER;

    body->CreateFixture(&fixtureDef);

    initBody(body, SENSOR, this);
}

void Director::cleanScene(b2World* world)
{
    if(world == nullptr) return;

    for (b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext())
    {
        b2Body* nextBody = body->GetNext();

        UserdataTag* tag = reinterpret_cast<UserdataTag*>(body->GetUserData().pointer);

        delete tag;

        world->DestroyBody(body);

        body = nextBody;
    }

    if(world != nullptr)
    {
        delete world;
        world = nullptr;
    }

    if(ground != nullptr)
    {
        delete ground;
        ground = nullptr;
    }
}

void Director::render()
{
    window.clear();
    currentScene->render(window);
    if(drawNpcs)
    {
    }
    if(drawPlayer)
    {
        view.setCenter(player->getPos().x + 33.f, 570);
        window.setView(view);
        player->draw(window);
    }
    if(world!=nullptr)
    {
        world->DebugDraw();
        //std::cout << "Dibujando debug" << std::endl;
    }

    window.display();
}

void Director::update(float deltaTime)
{
    if(currentScene->shouldTransition() && currentSceneState != SceneState::House)
    {
        switchScene(SceneState::House);
    }
    else if(currentScene->shouldTransition() && currentSceneState != SceneState::MainMenu)
    {
        switchScene(SceneState::MainMenu);
    }

    if(!oscureciendo)
    {
        currentScene->update(window, deltaTime);
    }
}

void Director::fadeOut()
{
    if(oscureciendo)
    {
        if(alpha < 255)
        {
            alpha += 1;
            fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
        }
        else
        {
            oscureciendo = false;
            initHouseScene();
            aclarando = true;
            alpha = 255;
        }
    }
}

void Director::fadeIn()
{
     if(aclarando)
    {
        if(alpha > 1)
        {
            alpha -= 1;
            fadeRectangle.setFillColor(sf::Color(0, 0, 0, alpha));
        }
        else
        {
            aclarando = false;
            transitioning = false;
        }
    }
}

void Director::switchScene(SceneState newScene)
{
    if (currentScene != nullptr)
    {
        delete currentScene;
        currentScene = nullptr;
    }

    currentSceneState = newScene;

    switch(currentSceneState)
    {
        case SceneState::MainMenu:
            initMenuScene();
            break;
        case SceneState::House:
            fadeOut();
            initHouseScene();
            fadeIn();
            break;
        case SceneState::City:
            initMenuScene();
            break;
    }
}

void Director::updateScene(float deltaTime)
{
    if(world != nullptr)
    {
        world->Step(timeStep, velocityIterations, positionIterations);
    }

    if(currentSceneState == SceneState::MainMenu && currentScene == nullptr)
    {
        initMenuScene();
    }
    else if(currentSceneState == SceneState::House && currentScene == nullptr)
    {
        initHouseScene();
    }

    if(currentScene != nullptr)
    {
        currentScene->update(window, deltaTime);
    }

    if(drawPlayer)
    {
        player->updateAnimation(deltaTime);
    }
}

void Director::gameEvents(float deltaTime)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        player->debug();
    }
    if(window.hasFocus() && player != nullptr)
    {
        player->keyboardInput(deltaTime);
    }
    if(drawNpcs)
    {
    }
}
