#ifndef OBSERVER_HPP_INCLUDED
#define OBSERVER_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <algorithm>

enum class ObserverEvents
{
    TRANSITION,
    SENSOR,
    DEFAULT
};

class Observer
{
    public:
        virtual ~Observer() = default;
        virtual void notify(ObserverEvents) = 0;
        virtual void sensorNotify(bool) {};
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

        void sensorTrigger(bool v)
        {
            for(Observer* observer : observers)
            {
                observer->sensorNotify(v);
            }
        }
};

#endif // OBSERVER_HPP_INCLUDED
