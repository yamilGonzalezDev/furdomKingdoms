#ifndef ANIMATIONMANAGER_H_INCLUDED
#define ANIMATIONMANAGER_H_INCLUDED

#include <unordered_map>
#include <string>
#include "animations.hpp"

class AnimationManager {
    private:
        std::unordered_map<std::string, Animation> animations;
        Animation* currentAnimation = nullptr;
        sf::Sprite& sprite;

    public:
        AnimationManager(sf::Sprite& spriteRef) : sprite(spriteRef) {}

        void addAnimation(std::string& name, Animation& animation)
        {
            animations[name] = animation;
        }

        void setAnimation(std::string& name)
        {
            auto it = animations.find(name);
            if (it != animations.end()) {
                if (currentAnimation != &it->second) {
                    currentAnimation = &it->second;
                    currentAnimation->reset();
                }
            }
        }

        void update(float deltaTime)
        {
            if (currentAnimation) {
                currentAnimation->update(deltaTime);
                sprite.setTextureRect(currentAnimation->getCurrentFrame());
            }
        }

        bool hasAnimation(std::string& name)
        {
            return animations.find(name) != animations.end();
        }
};

#endif // ANIMATIONMANAGER_H_INCLUDED
