#include "observer.h"

namespace pixpaint
{
namespace patterns
{
  void Subject::registerObserver(Observer& observer)
  {
    m_observers.push_back(&observer);
  }

  void Subject::notifyObservers()
  {
    for(auto* observer : m_observers) {
      observer->updateObserver();
    }
  }
}
}
