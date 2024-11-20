#ifndef OBSERVER_HPP_INCLUDED
#define OBSERVER_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>

enum class ObserverEvents
{
    Cinematic,
    Colision,
    Pause,
    Default
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
        void addObserver(Observer* observer)
        {
            observers.push_back(observer);
        }

        void removeObserver(Observer* observer)
        {                                            ///std::vector = {observer1, observer3, observer2}
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
