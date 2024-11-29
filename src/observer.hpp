#ifndef OBSERVER_HPP_INCLUDED
#define OBSERVER_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>
#include "observer.hpp"

enum class ObserverEvents
{
    TRANSITION,
    DEFAULT
};

class Observer
{
    public:
        virtual ~Observer() = default;
        virtual void notify(ObserverEvents) = 0;
};

class Subject
{
    private:
        std::vector<Observer*> observers;
    public:
        ~Subject()
        {
            observers.clear();
        }

        void addObserver(Observer* observer)
        {
            if(observer == nullptr)
            {
                std::cerr << "Error: Intentaste agregar un observador nulo" << std::endl;
                return;
            }
            observers.push_back(observer);
        }

        void removeObserver(Observer* observer)
        {
            observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        }

        void debugSubject()
        {
            std::cout << "Funciono" << std::endl;
            for(auto* observer : observers)
            {
                std::cout << "Ingreso al for" << std::endl;
                std::cout << "Puntero observer: " << observer << std::endl;
                if(observer == nullptr)
                {
                    std::cerr << "Error: Observador nulo en la lista" << std::endl;
                }
            }
        }

        void eventTrigger(ObserverEvents event)
        {
            for (Observer* observer : observers)
            {
                observer->notify(event);
            }
        }
};

#endif // OBSERVER_HPP_INCLUDED
