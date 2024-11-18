#ifndef ANIMATIONS_HPP_INCLUDED
#define ANIMATIONS_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
    private:
        std::vector<sf::IntRect> frames;
        int currentFrame = 0;
        float frameDuration;
        float elapsedTime = 0.f;
    public:
        Animation(std::vector<sf::IntRect>& frames, float duration)
        : frames(frames), frameDuration(duration) {}

        void update(float deltaTime)
        {
            if(frames.empty()) return;

            elapsedTime += deltaTime;
            if(elapsedTime >= frameDuration)
            {
                elapsedTime = 0.f;
                currentFrame = (currentFrame + 1) % frames.size();
            }
        }

        void reset() {
            currentFrame = 0;
            elapsedTime = 0.f;
        }
};

#endif // ANIMATIONS_HPP_INCLUDED
