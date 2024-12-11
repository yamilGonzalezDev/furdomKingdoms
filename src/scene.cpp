#include "scene.hpp"
#include "entity.hpp"

/**MENU**/
MenuScene::MenuScene()
{
    drawPlayer = false;

    if(!backgroundTexture.loadFromFile("Textures/menu/scroll.png"))
    {
        std::cerr << "Error cargando las texturas del menu" << std::endl;
        return;
    }

    if(!font.loadFromFile("Textures/font/menuFont.ttf"))
    {
        std::cerr << "Error al cargar la fuente del menú" << std::endl;
        return;
    }

    for(int i = 0; i < 3; i++)
    {
        menuOptions[i].setFont(font);
        menuOptions[i].setCharacterSize(50);
        menuOptions[i].setOutlineThickness(2.f);
    }

    float totalHeight = menuOptions[0].getGlobalBounds().height + 70.f;

    menuOptions[0].setString("Start");
    menuOptions[0].setOrigin(menuOptions[0].getGlobalBounds().width / 2, menuOptions[0].getGlobalBounds().height / 2);
    menuOptions[0].setPosition(WIDTH / 2, HEIGHT / 2 - totalHeight * 2);
    menuOptions[0].setFillColor(sf::Color::Yellow);

    menuOptions[1].setString("Settings");
    menuOptions[1].setOrigin(menuOptions[1].getGlobalBounds().width / 2, menuOptions[1].getGlobalBounds().height / 2);
    menuOptions[1].setPosition(WIDTH / 2, HEIGHT / 2 - 25);
    menuOptions[1].setFillColor(sf::Color::White);

    menuOptions[2].setString("Exit");
    menuOptions[2].setOrigin(menuOptions[2].getGlobalBounds().width / 2, menuOptions[1].getGlobalBounds().height / 2);
    menuOptions[2].setPosition(WIDTH / 2, HEIGHT / 2 + totalHeight);
    menuOptions[2].setFillColor(sf::Color::White);

    background.setTexture(backgroundTexture);
    background.setScale(13.f, 13.f);
    background.setOrigin(background.getLocalBounds().width / 2, background.getLocalBounds().height / 2);
    background.setPosition(WIDTH / 2, HEIGHT / 2);
}

void MenuScene::update(sf::RenderWindow& window, float deltaTime)
{
    const float cooldown = 0.4f;
    static float currentCooldown = 0.f;

    currentCooldown -= deltaTime;

    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))&& currentCooldown <= 0.f)
    {
        if (currentOption > 0)
            currentOption--;
        else
            currentOption = 2;

        currentCooldown = cooldown;
    }
    else if((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && currentCooldown <= 0.f)
    {
        if (currentOption < 2)
            currentOption++;
        else
            currentOption = 0;

        currentCooldown = cooldown;
    }

    for(int i = 0; i < 3; i++)
    {
        if(i == currentOption)
            menuOptions[i].setFillColor(sf::Color::Yellow);
        else
            menuOptions[i].setFillColor(sf::Color::White);
    }
}

void MenuScene::render(sf::RenderWindow& window)
{
    window.draw(background);

    window.draw(menuOptions[0]);
    window.draw(menuOptions[1]);
    window.draw(menuOptions[2]);
}

bool MenuScene::shouldTransition() const
{
    if(currentOption == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        return true;
    }
    return false;
}

void MenuScene::notify(ObserverEvents event)
{
    if(event != ObserverEvents::DEFAULT) return;
}

SceneState MenuScene::nextSceneState() const
{
    return SceneState::HOUSE;
}

/**CASA**/

HouseScene::HouseScene()
{
    if(!candleText.loadFromFile("Textures/houseLevel/candle.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!houseText.loadFromFile("Textures/houseLevel/house.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!bedText.loadFromFile("Textures/houseLevel/bed.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!tableText.loadFromFile("Textures/houseLevel/table.png"))
    {
        std::cerr << "Error al cargar las texturas de la casa" << std::endl;
    }
    if(!layerTexture5.loadFromFile("Textures/houseLevel/layer5.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!layerTexture4.loadFromFile("Textures/houseLevel/layer4.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!layerTexture3.loadFromFile("Textures/houseLevel/layer3.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!layerTexture2.loadFromFile("Textures/houseLevel/layer2.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!layerTexture1.loadFromFile("Textures/houseLevel/layer1.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }

    layerTexture5.setRepeated(true);
    layerTexture4.setRepeated(true);
    layerTexture3.setRepeated(true);
    layerTexture2.setRepeated(true);
    layerTexture1.setRepeated(true);

    layer5.setTexture(layerTexture5);
    layer4.setTexture(layerTexture4);
    layer3.setTexture(layerTexture3);
    layer2.setTexture(layerTexture2);
    layer1.setTexture(layerTexture1);

    layer5.setScale(2.f, 2.f);
    layer4.setScale(2.f, 2.f);
    layer3.setScale(2.f, 2.f);
    layer2.setScale(2.f, 2.f);
    layer1.setScale(2.f, 2.f);

    candle.setTexture(candleText);
    candle.setTextureRect(sf::IntRect(0, 0, 12, 20));
    candle.setPosition(457.f, 662.f);

    house.setTexture(houseText);
    house.setPosition(305.f, 529.f);

    bed.setTexture(bedText);
    bed.setPosition(368.f, 673.f);

    table.setTexture(tableText);
    table.setPosition(450.f, 682.f);
}

void HouseScene::update(sf::RenderWindow& window, float deltaTime)
{
    sf::Vector2f cameraPos = window.getView().getCenter();

    layer5.setPosition(-fmod(cameraPos.x * parallaxFactor5, WIDTH), 130.f);
    layer4.setPosition(-fmod(cameraPos.x * parallaxFactor4, WIDTH), 145.f);
    layer3.setPosition(-fmod(cameraPos.x * parallaxFactor3, WIDTH), 150.f);
    layer2.setPosition(-fmod(cameraPos.x * parallaxFactor2, WIDTH), 695.f);
    layer1.setPosition(-fmod(cameraPos.x * parallaxFactor1, WIDTH), 680.f);

    layer5.setTextureRect(sf::IntRect(fmod(cameraPos.x * parallaxFactor5, WIDTH), 0, 9000, HEIGHT));
    layer4.setTextureRect(sf::IntRect(fmod(cameraPos.x * parallaxFactor4, WIDTH), 0, 9000, HEIGHT));
    layer3.setTextureRect(sf::IntRect(fmod(cameraPos.x * parallaxFactor3, WIDTH), 0, 9000, HEIGHT));
    layer2.setTextureRect(sf::IntRect(fmod(cameraPos.x * parallaxFactor2, WIDTH), 0, 9000, 70));
    layer1.setTextureRect(sf::IntRect(fmod(cameraPos.x * parallaxFactor1, WIDTH), 0, 9000, 77));
}

void HouseScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

void HouseScene::render(sf::RenderWindow& window)
{
    window.draw(layer5);
    window.draw(layer4);
    window.draw(layer3);
    window.draw(layer2);
    window.draw(house);
    window.draw(table);
    window.draw(candle);
    window.draw(bed);
    playerAnimations.draw(window);
    window.draw(layer1);
}

bool HouseScene::shouldTransition() const
{
    return transition;
}

void HouseScene::notify(ObserverEvents event)
{
    if(transition) return;

    if(event == ObserverEvents::TRANSITION)
    {
        transition = true;
    }
}

SceneState HouseScene::nextSceneState() const
{
    return SceneState::CITY;
}

/**CIUDAD**/

CityScene::CityScene()
{
    drawPlayer = false;

    if(!backgroundTexture.loadFromFile("Textures/cityLevel/city.png"))
    {
        std::cerr << "Error al cargar las texturas de la ciudad" << std::endl;
    }

    background.setTexture(backgroundTexture);
    background.setPosition(0.f, 450.f);
}

void CityScene::update(sf::RenderWindow& window, float deltaTime)
{
    if(sensorActive && sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        transition = true;
    }
}

void CityScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

void CityScene::render(sf::RenderWindow& window)
{
    window.draw(background);
    playerAnimations.draw(window);
}

bool CityScene::shouldTransition() const
{
    return transition;
}

void CityScene::sensorNotify(bool v)
{
    sensorActive = v;
}

void CityScene::notify(ObserverEvents event)
{
    if(event == ObserverEvents::DEFAULT) std::cout << "Default" << std::endl;
}

SceneState CityScene::nextSceneState() const
{
    return SceneState::BAR;
}

/**BAR**/

BarScene::BarScene()
{
    if(!barTexture.loadFromFile("Textures/barLevel/bar.png"))
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    bar.setTexture(barTexture);
}

void BarScene::update(sf::RenderWindow& window, float deltaTime)
{

}

void BarScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

void BarScene::render(sf::RenderWindow& window)
{
    window.draw(bar);
    playerAnimations.draw(window);
}

SceneState BarScene::nextSceneState() const
{
    return SceneState::DEFAULT;
}

void BarScene::notify(ObserverEvents event)
{
    if(event == ObserverEvents::TRANSITION) transition = true;
}

bool BarScene::shouldTransition() const
{
    return transition;
}

/**TEST**/

TestScene::TestScene()
{
}

void TestScene::notify(ObserverEvents event)
{
    if(event != ObserverEvents::DEFAULT) return;
}

void TestScene::render(sf::RenderWindow& window)
{
    for(auto& enemy : enemyReferences)
    {
        window.draw(enemy->sprite);
    }

    playerAnimations.draw(window);

    theWitch.render(window);
}

void TestScene::update(sf::RenderWindow& window, float deltaTime)
{
    for(auto it = enemyReferences.begin(); it != enemyReferences.end();)
    {
        if(!(*it)->getIsAlive())
        {
            it = enemyReferences.erase(it);
        }
        else
        {
            ++it;
        }
    }

    theWitch.update(deltaTime);
}

void TestScene::updatePlayer(float deltaTime, b2Vec2 pos, sf::Vector2f spriteScale, PlayerState state)
{
    playerAnimations.update(deltaTime, pos, spriteScale, state);
}

bool TestScene::gameOverBool()
{
    return playerAnimations.getGameOver();
}

bool TestScene::shouldTransition() const
{
    return false;
}

SceneState TestScene::nextSceneState() const
{
    return SceneState::DEFAULT;
}

void TestScene::setEnemySprites(const std::vector<Enemy*>& enemyReferences)
{
    this->enemyReferences = enemyReferences;
}
