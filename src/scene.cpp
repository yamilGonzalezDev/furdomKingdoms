#include "scene.hpp"
#include "entity.hpp"

/**MENU**/
MenuScene::MenuScene()
{
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
    return SceneState::House;
}

/**CASA**/

HouseScene::HouseScene()
{
    if(!backgroundTexture.loadFromFile("Textures/houseLevel/casaNueva.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!farTexture.loadFromFile("Textures/houseLevel/layer2.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!midTexture.loadFromFile("Textures/houseLevel/layer3.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!nearTexture.loadFromFile("Textures/houseLevel/layer4.png"))
    {
        std::cerr << "Error cargando las texturas de la casa" << std::endl;
    }
    if(!candleText.loadFromFile("Textures/houseLevel/candle.png"))
    {
        std::cerr << "Error cargando las texturas de la vela" << std::endl;
    }
    candle.setTexture(candleText);
    candle.setTextureRect(sf::IntRect(0, 0, 12, 20));
    candle.setPosition(457.f , 662.f);
    background.setTexture(backgroundTexture);
    background.setPosition(0.f, 0.f);

    farTexture.setRepeated(true);
    midTexture.setRepeated(true);
    nearTexture.setRepeated(true);

    farSprite.setTexture(farTexture);
    midSprite.setTexture(midTexture);
    nearSprite.setTexture(nearTexture);

    farSprite.setScale(4.f, 2.f);
    midSprite.setScale(4.f, 2.f);
    nearSprite.setScale(4.f, 2.f);
}

void HouseScene::update(sf::RenderWindow& window, float deltaTime)
{
    sf::Vector2f cameraPos = window.getView().getCenter();

    float thresholdX = window.getSize().x * 0.50f;

    float texturePositionXFar = 0.f;
    if(cameraPos.x > thresholdX)
    {
        texturePositionXFar = (cameraPos.x - thresholdX) * parallaxFactorFar;
    }
    if(texturePositionXFar < 0)
    {
        texturePositionXFar = 0;
    }
    farSprite.setPosition(texturePositionXFar, 200.f);

    float texturePositionXMid = 0.f;
    if(cameraPos.x > thresholdX)
    {
        texturePositionXMid = (cameraPos.x - thresholdX) * parallaxFactorMid;
    }
    midSprite.setPosition(texturePositionXMid, 200.f);

    float texturePositionXNear = 0.f;
    if(cameraPos.x > thresholdX)
    {
        texturePositionXNear = (cameraPos.x - thresholdX) * parallaxFactorNear;
    }

    if(texturePositionXNear < 0)
    {
        texturePositionXNear = 0;
    }
    nearSprite.setPosition(texturePositionXNear, 200.f);
}

void HouseScene::render(sf::RenderWindow& window)
{
    window.draw(farSprite);
    window.draw(midSprite);
    window.draw(nearSprite);

    window.draw(background);
    window.draw(rect);
    window.draw(candle);
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
    return SceneState::City;
}

/**CIUDAD**/

CityScene::CityScene()
{
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

void CityScene::render(sf::RenderWindow& window)
{
    window.draw(background);
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
    return SceneState::Bar;
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

void BarScene::render(sf::RenderWindow& window)
{
    window.draw(bar);
}

SceneState BarScene::nextSceneState() const
{
    return SceneState::Default;
}

void BarScene::notify(ObserverEvents event)
{
    if(event == ObserverEvents::TRANSITION) transition = true;
}

bool BarScene::shouldTransition() const
{
    return transition;
}
