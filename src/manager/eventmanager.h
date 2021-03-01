/**********
 *
 *     PixPaint
 *     Copyright 2020-2021 Marvin Manese
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *
 **********/
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <vector>
#include "../3rdparty/ska/unordered_map.hpp"
#include "../pattern/singleton.h"

namespace pixpaint
{
  template<class EventT> class EventListener;
namespace event_manager_detail
{
  class EventListenerBase
  {
    friend class EventManager;

    virtual void emit(const void* event) = 0;

  public:
    virtual ~EventListenerBase() {}
  };

  class EventManager
  {
    PIXPAINT_SINGLETON_FUNC_FRIEND(EventManager)
    EventManager() {}

  public:
    template<class EventT> void registerListener(EventListener<EventT>& listener);
    template<class EventT> void emit(const EventT& event);

  private:
    ska::unordered_map<std::size_t, std::vector<EventListenerBase*>> m_listeners;
  };

  PIXPAINT_SINGLETON_FUNC(EventManager)
}
  template<class EventT>
  class EventListener : public event_manager_detail::EventListenerBase
  {
  public:
    EventListener();

    virtual void onEmit(const EventT& event) = 0;

  private:
    void emit(const void* event) override final;
  };

  template<class EventT>
  void emitEvent(const EventT& event);
}

#include "eventmanager.inl"

#endif // EVENTMANAGER_H
