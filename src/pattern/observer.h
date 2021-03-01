#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>

namespace pixpaint
{
namespace patterns
{
  class Observer;

  class Subject
  {
  public:
    void registerObserver(Observer& observer);

  protected:
    void notifyObservers();

  private:
    std::vector<Observer*> m_observers;
  };

  class Observer
  {
  public:
    virtual void updateObserver() = 0;
  };
}
}

#endif // OBSERVER_H
