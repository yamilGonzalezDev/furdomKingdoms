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
            observers.push_back(observer);
        }

        void removeObserver(Observer* observer)
        {
            observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        }

        void eventTrigger(ObserverEvents event)
        {
            for(Observer* observer : observers)
            {
                observer->notify(event);
            }
        }
};

#endif // OBSERVER_HPP_INCLUDED
