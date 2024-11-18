#ifndef NPC_HPP
#define NPC_HPP

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <unordered_map>
#include "entity.hpp"



enum class NpcState
{
    Idle,
    Walking
};

struct NpcAnimation
{
    std::vector<sf::IntRect> frames;
    float frameDuration;

    NpcAnimation() {};

    NpcAnimation(std::vector<sf::IntRect> frames, float duration) : frames(frames), frameDuration(duration) {};
};

#endif // NPC_H
